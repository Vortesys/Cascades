/* * * * * * * *\
	DRAW.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		The header for control drawing functions.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once

/* Includes */
#include <wtypes.h>

/* Function Prototypes */
LRESULT NTStyleWindowProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam, _In_ WNDPROC DefWndProc);
VOID NTStyleDrawWindow(_In_ HWND hWnd, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Drawing Functions
VOID NTStyleGetWindowMetrics(VOID);
VOID NTStyleDrawWindowBorders(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowCaption(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowButtons(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowTitle(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
BOOL NTStyleDrawFrameControl(_In_ HDC hDC, _In_ RECT rc, _In_ UINT uType, _In_ UINT uState);
