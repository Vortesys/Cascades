/* * * * * * * *\
	CSCDHOOK.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Defines the entry point for the DLL application.
		User hook functions are modeled after an examination
		of wbload (WindowBlinds) by win32ss.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "cscdhook.h"
#include "draw.h"
#include "..\common\usrapihk.h"
#include "resource.h"
#include <dwmapi.h>
#include <uxtheme.h>
#include <shlwapi.h>

/* Global Variables */
USERAPIHOOK g_user32ApiHook;
BYTE gabDWPmessages[UAHOWP_MAX_SIZE];
BYTE gabMSGPmessages[UAHOWP_MAX_SIZE];
BYTE gabDLGPmessages[UAHOWP_MAX_SIZE];
BOOL g_bThemeHooksActive = FALSE;

/* * * *\
	DllMain -
		NT Style Hook's entry point
\* * * */
BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/* * * *\
	NtStyleDisableWindowTheme -
		de-themify that window
\* * * */
__declspec(dllexport) VOID APIENTRY NtStyleDisableWindowTheme(_In_ HWND hWnd)
{
	enum DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;

	HANDLE hProcess = NULL;
	DWORD dwProcessID = 0;
	INT cchProcessName = MAX_PATH;
	WCHAR pszProcessName[MAX_PATH] = L"";

	// DO NOT DO ANY OF THIS TO EXPLORER!!!
	// EXPLORER NEEDS DWM!!!
	if (GetWindowThreadProcessId(hWnd, &dwProcessID) == 0)
		return;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);

	if (QueryFullProcessImageName(hProcess, 0, pszProcessName, &cchProcessName))
	{
		if (StrStrI(pszProcessName, (LPWSTR)L"\\explorer.exe") == NULL)
		{
			// Nuke DWM
			DwmSetWindowAttributeDelay(hWnd,
				DWMWA_NCRENDERING_POLICY,
				&ncrp,
				sizeof(ncrp));

			// Nuke the theming
			SetWindowThemeDelay(hWnd, L"", L"");
			RedrawWindow(hWnd, NULL, NULL, RDW_FRAME);
		}
	}

	return;
}

/* * * *\
	NtStyleInstallUserHook -
		Load the User32 API hook.
\* * * */
__declspec(dllexport) BOOL CALLBACK NtStyleInstallUserHook()
{
	USERAPIHOOKINFO uah;

	uah.m_funname1 = L"NtStyleInitUserHook";
	uah.m_dllname1 = L"ntshk64.dll";
	uah.m_funname2 = L"NtStyleInitUserHook";
	uah.m_dllname2 = L"ntshk64.dll";

	return RegisterUserApiHook(&uah);
}

/* * * *\
	NtStyleInitUserHook -
		Initialize the User32 API hook.
\* * * */
__declspec(dllexport) BOOL CALLBACK NtStyleInitUserHook(UAPIHK State, PUSERAPIHOOK puah)
{
	// Don't initialize if the state isn't appropriate.
	if (!puah || State != uahLoadInit)
	{
		g_bThemeHooksActive = FALSE;
		return TRUE;
	}

	/* Store the original functions from user32 */
	g_user32ApiHook = *puah;

	puah->DefWindowProcA = NtStyleDefWindowProcA;
	puah->DefWindowProcW = NtStyleDefWindowProcW;
	puah->PreWndProc = NtStylePreWindowProc;
	puah->PostWndProc = NtStylePostWindowProc;
	puah->PreDefDlgProc = NtStyleDlgPreWindowProc;
	puah->PostDefDlgProc = NtStyleDlgPostWindowProc;
	puah->DefWndProcArray.MsgBitArray = gabDWPmessages;
	puah->DefWndProcArray.Size = UAHOWP_MAX_SIZE;
	puah->WndProcArray.MsgBitArray = gabMSGPmessages;
	puah->WndProcArray.Size = UAHOWP_MAX_SIZE;
	puah->DlgProcArray.MsgBitArray = gabDLGPmessages;
	puah->DlgProcArray.Size = UAHOWP_MAX_SIZE;

	puah->SetWindowRgn = NtStyleSetWindowRgn;
	puah->GetScrollInfo = NtStyleGetScrollInfo;
	puah->SetScrollInfo = NtStyleSetScrollInfo;

	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCPAINT);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCACTIVATE);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCMOUSEMOVE);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCMOUSELEAVE);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCHITTEST);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCLBUTTONDOWN);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCUAHDRAWCAPTION);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCUAHDRAWFRAME);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_SETTEXT);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_WINDOWPOSCHANGED);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_CONTEXTMENU);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_STYLECHANGED);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_SETICON);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_NCDESTROY);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_SYSCOMMAND);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_CTLCOLORMSGBOX);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_CTLCOLORBTN);
	UAH_HOOK_MESSAGE(puah->DefWndProcArray, WM_CTLCOLORSTATIC);

	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_CREATE);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_SETTINGCHANGE);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_DRAWITEM);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_MEASUREITEM);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_WINDOWPOSCHANGING);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_WINDOWPOSCHANGED);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_STYLECHANGING);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_STYLECHANGED);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_NCCREATE);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_NCDESTROY);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_NCPAINT);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_MENUCHAR);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_MDISETMENU);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_THEMECHANGED);
	UAH_HOOK_MESSAGE(puah->WndProcArray, WM_UAHINIT);

	puah->DlgProcArray.MsgBitArray = gabDLGPmessages;
	puah->DlgProcArray.Size = UAHOWP_MAX_SIZE;

	UAH_HOOK_MESSAGE(puah->DlgProcArray, WM_INITDIALOG);
	UAH_HOOK_MESSAGE(puah->DlgProcArray, WM_CTLCOLORMSGBOX);
	UAH_HOOK_MESSAGE(puah->DlgProcArray, WM_CTLCOLORBTN);
	UAH_HOOK_MESSAGE(puah->DlgProcArray, WM_CTLCOLORDLG);
	UAH_HOOK_MESSAGE(puah->DlgProcArray, WM_CTLCOLORSTATIC);
	UAH_HOOK_MESSAGE(puah->DlgProcArray, WM_PRINTCLIENT);

	return TRUE;
}

/* * * *\
	NtStyleRemoveUserHook -
		Initialize the User32 API hook.
\* * * */
__declspec(dllexport) BOOL CALLBACK NtStyleRemoveUserHook()
{
	return UnregisterUserApiHook();
}

/* * * *\
	NtStyleDefWindowProcA -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStyleDefWindowProcA(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	return NtStyleWndProc(hWnd, Msg, wParam, lParam, g_user32ApiHook.DefWindowProcA);
}

/* * * *\
	NtStyleDefWindowProcW -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStyleDefWindowProcW(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	return NtStyleWndProc(hWnd, Msg, wParam, lParam, g_user32ApiHook.DefWindowProcW);
}

/* * * *\
	NtStylePreWindowProc -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStylePreWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown)
{
	// TODO: SET/CHECK/MODIFY WINDOW REGION - ELIMINATE UGLY BASIC CORNERS!!!
	switch (Msg)
	{
	case WM_CREATE:
		NtStyleDisableWindowTheme(hWnd);
		break;
	}

	return 0;
}

/* * * *\
	NtStylePostWindowProc -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStylePostWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown)
{
	return 0;
}

/* * * *\
	NtStyleDlgPreWindowProc -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStyleDlgPreWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown)
{
	return 0;
}

/* * * *\
	NtStyleDlgPostWindowProc -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStyleDlgPostWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown)
{
	return 0;
}

/* * * *\
	NtStyleSetWindowRgn -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStyleSetWindowRgn(HWND hWnd, HRGN hRgn, BOOL bRedraw)
{
	return g_user32ApiHook.SetWindowRgn(hWnd, hRgn, bRedraw);
}

/* * * *\
	NtStyleGetScrollInfo -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStyleGetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi)
{
	// ReactOS UxTheme tracks the scrollbar itself - if we change the metrics of
	// anything then we're going to have to change some stuff up here.
	return g_user32ApiHook.GetScrollInfo(hwnd, fnBar, lpsi);
}

/* * * *\
	NtStyleSetScrollInfo -
		User32 stub.
\* * * */
static LRESULT CALLBACK NtStyleSetScrollInfo(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL bRedraw)
{
	return g_user32ApiHook.SetScrollInfo(hWnd, fnBar, lpsi, bRedraw);
}

/* * * *\
	NtStyleWndProc -
		NT Style's Window Procedure functions.
\* * * */
LRESULT CALLBACK NtStyleWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, WNDPROC DefWndProc)
{
	switch (Msg)
	{
	case WM_NCPAINT:
		//
		// WM_NCUAHDRAWCAPTION : wParam are DC_* flags.
		//
	case WM_NCUAHDRAWCAPTION:
		//
		// WM_NCUAHDRAWFRAME : wParam is HDC, lParam are DC_ACTIVE and or DC_REDRAWHUNGWND.
		//
	case WM_NCUAHDRAWFRAME:
	case WM_NCACTIVATE:
		//if ((GetWindowLongW(hWnd, GWL_STYLE) & WS_CAPTION) != WS_CAPTION)
			//return TRUE;
		//return TRUE;
	case WM_NCMOUSEMOVE:
	case WM_NCMOUSELEAVE:
	case WM_NCLBUTTONDOWN:
		//switch (wParam)
		//{
		//case HTMINBUTTON:
		//case HTMAXBUTTON:
		//case HTCLOSE:
		//{
			//ThemeHandleButton(hWnd, wParam);
			//return 0;
		//}
		//default:
			//return DefWndProc(hWnd, Msg, wParam, lParam);
		//}
	//case WM_NCHITTEST:
	//{
		//POINT Point;
		//Point.x = GET_X_LPARAM(lParam);
		//Point.y = GET_Y_LPARAM(lParam);
		//return DefWndNCHitTest(hWnd, Point);
	//}
	case WM_SYSCOMMAND:
	//{
		//if ((wParam & 0xfff0) == SC_VSCROLL ||
			//(wParam & 0xfff0) == SC_HSCROLL)
		//{
			//POINT Pt;
			//Pt.x = (short)LOWORD(lParam);
			//Pt.y = (short)HIWORD(lParam);
			//NC_TrackScrollBar(hWnd, wParam, Pt);
			//return 0;
		//}
		//else
		//{
			return DefWndProc(hWnd, Msg, wParam, lParam);
		//}
	//}
	default:
		return DefWndProc(hWnd, Msg, wParam, lParam);
	}
}
