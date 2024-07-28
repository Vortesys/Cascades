/* * * * * * * *\
	MAIN.C -
		Copyright © 2024 Brady McDermott
	DESCRIPTION -
		User32 UserApiHook testing program.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma warning(disable : 4996) // error C4996: 'GetVersion': was declared deprecated
#pragma warning(disable : 28159) // error C28159: 'GetVersion': was declared deprecated

/* Includes */
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#include "hook.h"
#include "thmfunc.h"
#include "svc.h"

/* Defines */
#define WM_THEMECHANGED 0x031A

/* Global Variables */
HMODULE g_hModule = NULL;
USERAPIHOOK g_user32ApiHook;
BYTE gabDWPmessages[UAHOWP_MAX_SIZE];
BYTE gabMSGPmessages[UAHOWP_MAX_SIZE];
BYTE gabDLGPmessages[UAHOWP_MAX_SIZE];
BOOL g_bThemeHooksActive = FALSE;

/* Functions */

/* * * *\
	InstallUserHook -
		Load the User32 API hook.
\* * * */
__declspec(dllexport) BOOL CALLBACK InstallUserHook()
{
	WCHAR szFullPath[MAX_PATH];
	USERAPIHOOKINFO uah;

	OutputDebugString(TEXT("InstallUserHook called\n"));

	// Unregister before we do anything
	// TODO: kill uxtheme kill uxtheme
	//UnregisterUserApiHookDelay();

	// Get the module
	g_hModule = GetModuleHandle(NULL);

	// Get our current directory and filename
	GetModuleFileName(g_hModule, szFullPath, ARRAYSIZE(szFullPath));


	// Fill out the ApiHookInfo structure
	uah.m_size = sizeof(uah);
	uah.m_funname1 = L"InitUserHook";
	uah.m_dllname1 = szFullPath;
	uah.m_funname2 = L"InitUserHook";
	uah.m_dllname2 = szFullPath;

	// ApiHook is not support on Windows
	// 2000 or below!
	return RegisterUserApiHookDelay(g_hModule, &uah);
}

/* * * *\
	InitUserHook -
		Initialize the User32 API hook.
\* * * */
__declspec(dllexport) BOOL CALLBACK InitUserHook(UAPIHK State, PUSERAPIHOOK puah)
{
	OutputDebugString(TEXT("InitUserHook called\n"));

	// Don't initialize if the state isn't appropriate.
	if (!puah || State != uahLoadInit)
	{
		g_bThemeHooksActive = FALSE;
		return TRUE;
	}

	OutputDebugString(TEXT("InitUserHook initializing\n"));

	/* Store the original functions from user32 */
	g_user32ApiHook = *puah;

	puah->DefWindowProcA = ThemeDefWindowProcA;
	puah->DefWindowProcW = ThemeDefWindowProcW;
	puah->PreWndProc = ThemePreWindowProc;
	puah->PostWndProc = ThemePostWindowProc;
	puah->PreDefDlgProc = ThemePreDefDlgProc;
	puah->PostDefDlgProc = ThemePostDefDlgProc;
	puah->DefWndProcArray.MsgBitArray = gabDWPmessages;
	puah->DefWndProcArray.Size = UAHOWP_MAX_SIZE;
	puah->WndProcArray.MsgBitArray = gabMSGPmessages;
	puah->WndProcArray.Size = UAHOWP_MAX_SIZE;
	puah->DlgProcArray.MsgBitArray = gabDLGPmessages;
	puah->DlgProcArray.Size = UAHOWP_MAX_SIZE;

	puah->SetWindowRgn = (FARPROC)ThemeSetWindowRgn;
	puah->GetScrollInfo = (FARPROC)ThemeGetScrollInfo;
	puah->SetScrollInfo = (FARPROC)ThemeSetScrollInfo;

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
	RemoveUserHook -
		Removes the User32 API hook.
\* * * */
__declspec(dllexport) BOOL CALLBACK RemoveUserHook(VOID)
{
	return UnregisterUserApiHookDelay();
}

/* * * *\
	RegisterUserApiHookDelay -
		Run the User32 API hook. Pick the
		correct version and then hook!
\* * * */
BOOL WINAPI RegisterUserApiHookDelay(HINSTANCE hInstance, PUSERAPIHOOKINFO ApiHookInfo)
{
	// TODO: use GetLastError!!!
	HMODULE hLib = LoadLibrary(L"user32.dll");
	BOOL bRet = 0;
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	DWORD dwVersion = 0;

	// Get the Windows version so that
	// we can use the appropriate structures.
	dwVersion = GetVersion();

	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	if (!hLib)
	{
		// Failed to load DLL
		OutputDebugString(TEXT("Failed to load user32\n"));

		return FALSE;
	}

	// Windows XP specific
	if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
	{
		USERAPIHOOKINFO_XP uah;
		RUAH_XP fLib = (RUAH_XP)GetProcAddress(hLib, "RegisterUserApiHook");

		if (fLib == NULL)
		{
			// Failed to load DLL
			OutputDebugString(TEXT("Failed to load RegisterUserApiHook\n"));

			return FALSE;
		}

		uah.hInstance = hInstance;
		uah.CallbackFunc = (FARPROC)InitUserHook;

		bRet = fLib(&uah);
	}
	// Server 2003 and newer
	else if (((dwMajorVersion == 5) && (dwMinorVersion == 2))
		|| (dwMajorVersion >= 6))
	{
		RUAH fLib = (RUAH)GetProcAddress(hLib, "RegisterUserApiHook");

		if (fLib == NULL)
		{
			// Failed to load DLL
			OutputDebugString(TEXT("Failed to load RegisterUserApiHook\n"));

			return FALSE;
		}

		bRet = fLib(ApiHookInfo);
	}

	FreeLibrary(hLib);

	return bRet;
}


/* * * *\
	UnregisterUserApiHookDelay -
		Unregisters a DLL from User32.
	RETURNS -
		TRUE if successful.
\* * * */
BOOL WINAPI UnregisterUserApiHookDelay(VOID)
{
	// TODO: use GetLastError!!!
	HMODULE hLib = LoadLibrary(L"user32.dll");
	BOOL bRet = 0;

	if (hLib)
	{
		FARPROC fLib = GetProcAddress(hLib, "UnregisterUserApiHook");

		bRet = (BOOL)fLib();

		FreeLibrary(hLib);

		return bRet;
	}

	// ApiHook is not support on Windows
	// 2000 or below!
	return FALSE;
}
