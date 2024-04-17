/* * * * * * * *\
	NTSHOOK.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style Hook's primary header file.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "UxTheme.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Shlwapi.lib")

/* Includes */
#include <wtypes.h>

/* Definitions */
#define STATUS_UNSUCCESSFUL 0xC0000001

/* Global Variables */

/* Function Prototypes */
// Hook Procedure
__declspec(dllexport) LRESULT APIENTRY NtStyleHookProc(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Functions
__declspec(dllexport) VOID APIENTRY NtStyleDisableWindowTheme(_In_ HWND hWnd);

// Delay Load Functions
HRESULT DwmGetWindowAttributeDelay(HWND hwnd, DWORD dwAttribute, _In_ LPCVOID pvAttribute, DWORD cbAttribute);
HRESULT DwmSetWindowAttributeDelay(HWND hwnd, DWORD dwAttribute, _In_ LPCVOID pvAttribute, DWORD cbAttribute);
HRESULT SetWindowThemeDelay(_In_ HWND hwnd, _In_ LPCWSTR pszSubAppName, _In_ LPCWSTR pszSubIdList);
