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
#include <Windows.h>

// Handles
HINSTANCE	g_hAppInstance;
HINSTANCE	g_hDllInstance;

/* Functions */

/* * * *\
	wWinMain -
		NT Style's entry point and message loop.
\* * * */
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	MSG msg = { 0 };
	HHOOK hkprcNTShook = NULL;

	// Get our own hInstance and save it for later
	g_hAppInstance = hInstance;

	// Load the hook DLL
	g_hDllInstance = LoadLibrary(L"ntshook.dll");

	// Get the hook procedure of NTShook
	if(g_hDllInstance)
		hkprcNTShook = (HOOKPROC)GetProcAddress(g_hDllInstance, "NTStyleHookProc");

	// Establish our hook :)
	if (hkprcNTShook)
		SetWindowsHookEx(WH_CALLWNDPROC, hkprcNTShook, g_hDllInstance, 0);
	// Use callnexthook later
		/*Before terminating, an application must call the UnhookWindowsHookEx function
		function to free system resources associated with the hook.*/

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		DispatchMessage(&msg);
	}

	return 0;
}