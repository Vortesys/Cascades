/* * * * * * * *\
	NTSHOOK.C -
		Copyright � 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Defines the entry point for the DLL application.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
 \* * * * * * * */

/* Headers */
#include "ntshook.h"
#include "ntsdrfn.h"
#include "resource.h"
#include <dwmapi.h>
#include <uxtheme.h>
#include <shlwapi.h>

/* Global Variables */
static INT g_iSystemHasDWM = 2;
static INT g_iLockUpdateCount;

// Handles
HINSTANCE	g_hDllInstance;

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
	NTStyleHookProc -
		NT Style Hook procedure
		Uses WH_CALLWNDPROC.
\* * * */
__declspec(dllexport) LRESULT APIENTRY NTStyleHookProc(
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	PCWPSTRUCT pcwps;

	// Switch case for window painting
	switch (uMsg)
	{
	/* BEGIN HC_ACTION */
	case HC_ACTION:
		pcwps = (CWPSTRUCT*)lParam;

		switch (pcwps->message)
		{
		// Drawing Messages
		case WM_CREATE:
			NTStyleDisableWindowTheme(pcwps->hwnd);
			break;

		case 0xAE: // WM_NCUAHDRAWCAPTION:
			// WM_NCUAHDRAWCAPTION : wParam are DC_* flags.
		case 0xAF: // WM_NCUAHDRAWFRAME:
			// WM_NCUAHDRAWFRAME : wParam is HDC, lParam are DC_ACTIVE and or DC_REDRAWHUNGWND.
		case WM_NCACTIVATE:
			//if ((GetWindowLongW(hWnd, GWL_STYLE) & WS_CAPTION) != WS_CAPTION)
				//return TRUE;

			NTStyleDrawWindow(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
			return TRUE;

		case WM_NCPAINT:
			NTStyleDrawWindow(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
			return 0;
		
		// Collision Messages
			/*
		case WM_NCLBUTTONUP:
			break;
			
		case WM_NCLBUTTONDBLCLK:
			break;
			
		case WM_NCRBUTTONUP:
			break;
			
		case WM_NCHITTEST:
			break;*/

		default:
			break;
		}
	/* END HC_ACTION */

	default:
		break;
	}

	return CallNextHookEx(NULL, uMsg, wParam, lParam);
}

/* * * *\
	NTStyleSetHook -
		NT Style Hook hook so
		that we can actually
		WOW these programs.
\* * * */
__declspec(dllexport) HHOOK APIENTRY NTStyleSetHook(_In_ INT idHook, _In_ HOOKPROC lpfn, _In_ HINSTANCE hmod, _In_ DWORD dwThreadId)
{
	return SetWindowsHookEx(idHook, lpfn, hmod, dwThreadId);
}

/* * * *\
	NTStyleDisableWindowTheme -
		de-themify that window
\* * * */
__declspec(dllexport) VOID APIENTRY NTStyleDisableWindowTheme(_In_ HWND hWnd)
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
