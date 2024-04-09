/* * * * * * * *\
	NTSDRFN.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's drawing functions.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once

/* Includes */
#include <wtypes.h>

/* Definitions */
// NT Style Draw Frame Control Types
#define NSDFC_TYPE_BUTTON 0x00 // Standard window button
#define NSDFC_TYPE_CAPTION 0x01 // Caption bar button
// NT Style Draw Frame Control Styles
#define NSDFC_STYLE_NONE 0x00 // Standard
#define NSDFC_STYLE_PRESSED 0x01 // Pressed
#define NSDFC_STYLE_ACTIVE 0x02 // Active control
#define NSDFC_STYLE_MINIMIZE 0x10 // Minimized caption arrow
#define NSDFC_STYLE_MAXIMIZE 0x20 // Maximized caption arrow

/* Function Prototypes */
VOID NTStyleDrawWindow(_In_ HWND hWnd, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Drawing Functions
VOID NTStyleGetWindowMetrics(VOID);
VOID NTStyleDrawWindowBorders(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowCaption(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowButtons(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
VOID NTStyleDrawWindowTitle(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam);
BOOL NTStyleDrawFrameControl(_In_ HDC hDC, _In_ RECT rc, _In_ UINT uType, _In_ UINT uState);
