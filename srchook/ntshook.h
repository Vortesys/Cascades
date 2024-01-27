/* * * * * * * *\
	NTSTYLE.H -
		Copyright © 2023 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's primary header file.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "UxTheme.lib")

/* Includes */
#include <wtypes.h>

/* Global Variables */

/* Function Prototypes */
// Hook Procedure
__declspec(dllexport) LRESULT NTStyleHookProc(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Functions
VOID NTStyleDrawWindow(_In_ HWND hWnd, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleLockWindowUpdates(_In_ HWND hWnd);
VOID NTStyleUnlockWindowUpdates(_In_ HWND hWnd);
__declspec(dllexport) VOID NTStyleDisableWindowTheme(_In_ HWND hWnd);

// Drawing Functions
VOID NTStyleGetWindowMetrics(VOID);
VOID NTStyleDrawWindowBorders(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowCaption(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowButtons(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowTitle(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Delay Load Functions
HRESULT DwmSetWindowAttributeDelay(HWND hwnd, DWORD dwAttribute, _In_ LPCVOID pvAttribute, DWORD cbAttribute);
