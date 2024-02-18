/* * * * * * * *\
	NTSHOOK.H -
		Copyright © 2023 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style Hook's primary header file.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "UxTheme.lib")
#pragma comment(lib, "Msimg32.lib")

/* Includes */
#include <wtypes.h>

/* Global Variables */

/* Function Prototypes */
// Hook Procedure
__declspec(dllexport) LRESULT APIENTRY NTStyleHookProc(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
__declspec(dllexport) HHOOK APIENTRY NTStyleSetHook(_In_ INT idHook, _In_ HOOKPROC lpfn, _In_ HINSTANCE hmod, _In_ DWORD dwThreadId);

// Functions
VOID NTStyleLockWindowUpdates(_In_ HWND hWnd);
VOID NTStyleUnlockWindowUpdates(_In_ HWND hWnd);
__declspec(dllexport) VOID APIENTRY NTStyleDisableWindowTheme(_In_ HWND hWnd);

// Delay Load Functions
HRESULT DwmSetWindowAttributeDelay(HWND hwnd, DWORD dwAttribute, _In_ LPCVOID pvAttribute, DWORD cbAttribute);
