/* * * * * * * *\
	NTSTYLE.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's main file.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "ntstyle.h"
#include "resource.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

// Handles
HINSTANCE	g_hAppInstance;
HHOOK		g_hhkNTShk32 = NULL;
HHOOK		g_hhkNTShk64 = NULL;
// Strings
WCHAR		g_szAppTitle[64];
// Other
BOOL g_bSystem64 = TRUE;

/* Functions */

/* * * *\
	wWinMain -
		NT Style's entry point and message loop.
\* * * */
int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	MSG msg = { 0 };
	HWND hDlg;
	SYSTEM_INFO si;

	// Get our own hInstance and save it for later
	g_hAppInstance = hInstance;

	// Create our main window dialog
	InitCommonControls();
	hDlg = CreateDialogParam(g_hAppInstance, MAKEINTRESOURCE(IDD_MAIN), 0, NtStyleDialogProc, 0);
	ShowWindow(hDlg, nCmdShow);

	// Get some system information to determine what
	// copies of the style hook to load
	GetSystemInfo(&si);

	g_bSystem64 = (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);

	// Enter the dialog message loop.
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!IsDialogMessage(hDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

/* * * *\
	NtStyleCreateHook -
		NT Style's hook creation function.
\* * * */
DWORD NtStyleCreateHook(
	_In_ HINSTANCE hInst,
	_In_ LPWSTR lpNTStyleHook,
	_In_ BOOL bDisableTheming,
	_Out_ HHOOK hhkNTShk
)
{
	HINSTANCE hDllInstance = NULL;
	HOOKPROC hkprc = NULL;
	DWORD dwLastError = 0;

	hhkNTShk = NULL;

	// Load the hook DLL
	hDllInstance = LoadLibrary(lpNTStyleHook);

	// Get the hook procedure of NTShook
	if (hDllInstance)
		hkprc = (HOOKPROC)GetProcAddress(hDllInstance, "NtStyleHookProc");
	else
		dwLastError = GetLastError();

	// Establish our hook
	if (hkprc)
		hhkNTShk = SetWindowsHookEx(WH_CALLWNDPROC, hkprc, hDllInstance, 0);
	else
		dwLastError = GetLastError();

	// Enumerate the existing windows and get them dwm-free :fire:
	if (bDisableTheming)
		EnumWindows(&NtStyleEnumWindowProc, (LPARAM)hDllInstance);

	if (hDllInstance)
		FreeLibrary(hDllInstance);

	return dwLastError;
}

/* * * *\
	NtStyleEnumWindowProc -
		NT Style's window enumeration procedure.
\* * * */
BOOL CALLBACK NtStyleEnumWindowProc(
	_In_ HWND hwnd,
	_In_ LPARAM lParam
)
{
	if ((HMODULE)lParam != NULL)
	{
		FARPROC fLib = GetProcAddress((HMODULE)lParam, "NtStyleDisableWindowTheme");
		fLib(hwnd);

		return TRUE;
	}

	return FALSE;
}
