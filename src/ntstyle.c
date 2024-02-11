/* * * * * * * *\
	NTSTYLE.C -
		Copyright © 2023 Brady McDermott, Vortesys
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
HINSTANCE	g_hDll32Instance;
HINSTANCE	g_hDll64Instance;

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

	HOOKPROC hkprcNTShk32 = NULL;
	HOOKPROC hkprcNTShk64 = NULL;
	HHOOK hhkNTShk32 = NULL;
	HHOOK hhkNTShk64 = NULL;

	// Get our own hInstance and save it for later
	g_hAppInstance = hInstance;

	// Load the 32-bit hook DLL
	g_hDll32Instance = LoadLibrary(L"ntshk32.dll");

	// Get the hook procedure of NTShook
	if (g_hDll32Instance)
		hkprcNTShk32 = (HOOKPROC)GetProcAddress(g_hDll32Instance, "NTStyleHookProc");
	else
		return GetLastError();

	// Establish our WOW hook :)
	if (hkprcNTShk32)
		hhkNTShk32 = SetWindowsHookEx(WH_CALLWNDPROC, hkprcNTShk32, g_hDll32Instance, 0);
	else
		return GetLastError();

	// Load the 64-bit hook DLL
	g_hDll64Instance = LoadLibrary(L"ntshk64.dll");

	// Get the hook procedure of NTShook
	if (g_hDll64Instance)
		hkprcNTShk64 = (HOOKPROC)GetProcAddress(g_hDll64Instance, "NTStyleHookProc");
	else
		return GetLastError();

	// Establish our hook :)
	if (hkprcNTShk64)
		hhkNTShk64 = SetWindowsHookEx(WH_CALLWNDPROC, hkprcNTShk64, g_hDll64Instance, 0);
	else
		return GetLastError();
		
	if (hhkNTShk32 && hhkNTShk64)
	{
		// Enumerate the existing windows and get them dwm-free :fire:
		EnumWindows(&NTStyleEnumWindowProc, 0);

		// Start NT Style
		if (!hhkNTShk32)
			MessageBox(HWND_DESKTOP, L"Started NT Style.\nPress OK to close.", L"NT Style (AMD64)", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_DEFAULT_DESKTOP_ONLY);
		else if (!hhkNTShk64)
			MessageBox(HWND_DESKTOP, L"Started NT Style.\nPress OK to close.", L"NT Style (IA32)", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_DEFAULT_DESKTOP_ONLY);
		else
			MessageBox(HWND_DESKTOP, L"Started NT Style.\nPress OK to close.", L"NT Style (AMD64 & WOW64)", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL | MB_DEFAULT_DESKTOP_ONLY);
	}

	if (hhkNTShk32)
		UnhookWindowsHookEx(hhkNTShk32);

	if (hhkNTShk64)
		UnhookWindowsHookEx(hhkNTShk64);

	MessageBox(HWND_DESKTOP, L"Quitting NT Style...", L"NT Style", MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);

	PostQuitMessage(0);

	return 0;
}

BOOL CALLBACK NTStyleEnumWindowProc(
	_In_ HWND hwnd,
	_In_ LPARAM lParam
)
{
	if (g_hDll64Instance)
	{
		FARPROC fLib = GetProcAddress(g_hDll64Instance, "NTStyleDisableWindowTheme");
		fLib(hwnd);
		
	}
	else if (g_hDll32Instance)
	{
		FARPROC fLib = GetProcAddress(g_hDll32Instance, "NTStyleDisableWindowTheme");
		fLib(hwnd);

	}

	return TRUE;
}