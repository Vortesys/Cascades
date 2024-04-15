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

// Handles
HINSTANCE	g_hAppInstance;
HHOOK		g_hhkNTShk32 = NULL;
HHOOK		g_hhkNTShk64 = NULL;

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

	FARPROC fNTSWOW = NULL;

	DWORD dwLastError = 0;
	BOOL bSystem64 = TRUE;

	SYSTEM_INFO si;

	// Get our own hInstance and save it for later
	g_hAppInstance = hInstance;

	// Get some system information to determine what
	// copies of the style hook to load
	GetSystemInfo(&si);

	bSystem64 = (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64);

	if (bSystem64)
	{
		dwLastError = NTStyleCreateHook(g_hAppInstance, L"ntshk64.dll", g_hhkNTShk64);
	}

	// load that 32 bit ish
	dwLastError = NTStyleCreateHook(g_hAppInstance, L"ntshk32.dll", g_hhkNTShk32);
		
	if (g_hhkNTShk32 || g_hhkNTShk64)
	{
		// Start NT Style
		if (!g_hhkNTShk32)
			MessageBox(HWND_DESKTOP, L"Started NT Style.\nPress OK to close.", L"NT Style (AMD64)",
				MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_DEFAULT_DESKTOP_ONLY);
		else if (!g_hhkNTShk64)
			MessageBox(HWND_DESKTOP, L"Started NT Style.\nPress OK to close.", L"NT Style (IA32)",
				MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_DEFAULT_DESKTOP_ONLY);
		else
			MessageBox(HWND_DESKTOP, L"Started NT Style.\nPress OK to close.", L"NT Style (AMD64 + WOW)",
				MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_DEFAULT_DESKTOP_ONLY);
	}

	if (g_hhkNTShk32)
		UnhookWindowsHookEx(g_hhkNTShk32);
	if (g_hhkNTShk64)
		UnhookWindowsHookEx(g_hhkNTShk64);

	PostQuitMessage(0);

	return dwLastError;
}

/* * * *\
	NTStyleCreateHook -
		NT Style's hook creation function.
\* * * */
DWORD NTStyleCreateHook(
	_In_ HINSTANCE hInst,
	_In_ LPWSTR lpNTStyleHook,
	_Out_ HHOOK hhkNTShk
)
{
	HINSTANCE hDllInstance = NULL;
	HOOKPROC hkprc = NULL;

	DWORD dwLastError = 0;

	// Load the hook DLL
	hDllInstance = LoadLibrary(lpNTStyleHook);

	// Get the hook procedure of NTShook
	if (hDllInstance)
		hkprc = (HOOKPROC)GetProcAddress(hDllInstance, "NTStyleHookProc");
	else
		dwLastError = GetLastError();

	// Establish our hook
	if (hkprc)
		hhkNTShk = SetWindowsHookEx(WH_CALLWNDPROC, hkprc, hDllInstance, 0);
	else
		dwLastError = GetLastError();

	// Enumerate the existing windows and get them dwm-free :fire:
	EnumWindows(&NTStyleEnumWindowProc, (LPARAM)hDllInstance);

	if (hDllInstance)
		FreeLibrary(hDllInstance);

	return hhkNTShk;
}

/* * * *\
	NTStyleEnumWindowProc -
		NT Style's window enumeration procedure.
\* * * */
BOOL CALLBACK NTStyleEnumWindowProc(
	_In_ HWND hwnd,
	_In_ LPARAM lParam
)
{
	if (lParam != NULL)
	{
		FARPROC fLib = GetProcAddress(lParam, "NTStyleDisableWindowTheme");
		fLib(hwnd);

		return TRUE;
	}

	return FALSE;
}
