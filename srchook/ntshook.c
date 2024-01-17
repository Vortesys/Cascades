/* * * * * * * *\
	NTSTYLE.C -
		Copyright © 2023 Brady McDermott, Vortesys
	DESCRIPTION -
		Defines the entry point for the DLL application.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
 \* * * * * * * */

// Includes
#include "ntshook.h"
#include <strsafe.h>

// System Metrics
static UINT g_uiCaptionHeight;
static UINT g_uiBorderWidth;
static UINT g_uiBorderHeight;
static UINT g_uiDlgFrmWidth;
static UINT g_uiDlgFrmHeight;

// System Colors
static DWORD g_dwActiveCaption;
static DWORD g_dwActiveCaptionGradient;
static DWORD g_dwActiveCaptionText;
static DWORD g_dwActiveBorder;
static DWORD g_dwInactiveCaption;
static DWORD g_dwInactiveCaptionGradient;
static DWORD g_dwInactiveCaptionText;
static DWORD g_dwInactiveBorder;
static DWORD g_dwWindowFrame;

/* * * *\
	DllMain -
		NT Style Hook's entry point
\* * * */
BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/* * * *\
	NTStyleHookProc -
		NT Style Hook procedure
		Uses WH_CALLWNDPROC.
\* * * */
__declspec(dllexport) LRESULT NTStyleHookProc(
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	PCWPSTRUCT pcwps;

	// Switch case for window painting
	switch (uMsg)
	{
	/* BEGIN HC_ACTION */
	case HC_ACTION:
		pcwps = (CWPSTRUCT*)lParam;

		switch (pcwps->message)
		{
		case WM_NCCALCSIZE:
		case WM_NCACTIVATE:
		case WM_NCPAINT:
			if ((GetWindowLongPtr(pcwps->hwnd, GWL_STYLE) & WS_CAPTION) == WS_CAPTION)
			{
				NTStyleDrawWindowCaption(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
				NTStyleDrawWindowBorders(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
			}
			break;

		default:
			break;
		}
	/* END HC_ACTION */

	default:
		break;
	}

	return CallNextHookEx(NULL, uMsg, wParam, lParam);
}

/* * * *\
	NTStyleDrawWindowCaption -
		Draws window caption & title
\* * * */
VOID NTStyleDrawWindowCaption(_In_ HWND hWnd, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	/* If an application processes the WM_NCACTIVATE message, after
	processing it must return TRUE to direct the system to complete
	the change of active window. If the window is minimized when the
	application receives the WM_NCACTIVATE message, it should pass
	the message to DefWindowProc. In such cases, the default function
	redraws the label for the icon. */

	HDC hdc = NULL;
	HBRUSH hbr = NULL;

	if (hWnd != NULL)
		hdc = GetWindowDC(hWnd);

	// Always refresh the colors and metrics first
	NTStyleGetWindowMetrics();

	if (hdc)
	{

		ReleaseDC(hWnd, hdc);
	}
	
	return;
}

/* * * *\
	NTStyleDrawWindowBorders -
		Draws window borders
\* * * */
VOID NTStyleDrawWindowBorders(_In_ HWND hWnd, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	/* The system maintains an update region for the nonclient area.
	When an application receives a WM_NCPAINT message, the wParam
	parameter contains a handle to a region defining the dimensions of
	the update region. The application can use the handle to combine the
	update region with the clipping region for the window device context.
	The system does not automatically combine the update region when
	retrieving the window device context unless the application uses
	GetDCEx and specifies both the region handle and the DCX_INTERSECTRGN
	flag. If the application does not combine the update region, only
	drawing operations that would otherwise extend outside the window are
	clipped. The application is not responsible for clearing the update
	region, regardless of whether it uses the region. */

	HDC hdc = NULL;
	RECT rcWindow = { 0, 0, 0, 0 };
	HBRUSH hbr = NULL;
	PPOINT apt[40];
	INT asz[] = { 6, 4, 6, 4, 6, 4, 6, 4 };
	INT csz = 8;

	// Verify our window handle
	if (hWnd != NULL)
		hdc = GetWindowDC(hWnd);

	// Always refresh the colors and metrics before drawing
	NTStyleGetWindowMetrics();

	// Color our brushes

	// Verify the device context
	if (hdc)
	{
		hbr = GetSysColorBrush(COLOR_ACTIVECAPTION);

		// Get external window size
		get window rectangle here!!
		
		// Calculate the polygon points
		// this is the first one
		// make this programatically
		apt[0] = MAKEPOINTS(-g_uiBorderWidth , -g_uiBorderHeight);
		apt[1] = MAKEPOINTS(g_uiCaptionHeight, g_uiBorderHeight);
		apt[2] = MAKEPOINTS(0, g_uiCaptionHeight);
		apt[3] = MAKEPOINTS(0, 0);
		apt[4] = MAKEPOINTS(0, g_uiCaptionHeight);
		apt[5] = MAKEPOINTS(0, 0);
		apt[1] = MAKEPOINTS(0, 0);
		apt[1] = MAKEPOINTS(0, 0);


		// Set the polygon fill mode
		SetPolyFillMode(hdc, WINDING);


		DeleteObject(hbr);
		ReleaseDC(hWnd, hdc);
	}

	return;
}

/* * * *\
	NTStyleGetWindowMetrics -
		Gathers system colors
		and metrics for use with
		the drawing functions.
\* * * */
VOID NTStyleGetWindowMetrics(VOID)
{
	// First get the system metrics.
	g_uiCaptionHeight = GetSystemMetrics(SM_CYCAPTION); // Caption bar height
	g_uiBorderWidth = GetSystemMetrics(SM_CXBORDER); // Border width
	g_uiBorderHeight = GetSystemMetrics(SM_CYBORDER); // Border height
	g_uiDlgFrmWidth = GetSystemMetrics(SM_CXDLGFRAME); // Dialog frame width
	g_uiDlgFrmHeight = GetSystemMetrics(SM_CYDLGFRAME); // Dialog frame height

	// And then the active colors...
	g_dwActiveCaption = GetSysColor(COLOR_ACTIVECAPTION); // Active caption bar
	g_dwActiveCaptionGradient = GetSysColor(COLOR_GRADIENTACTIVECAPTION); // Active caption bar gradient
	g_dwActiveCaptionText = GetSysColor(COLOR_CAPTIONTEXT); // Active caption text
	g_dwActiveBorder = GetSysColor(COLOR_ACTIVEBORDER); // Active window border

	// Plus inactive...
	g_dwInactiveCaption = GetSysColor(COLOR_INACTIVECAPTION); // Inactive caption bar
	g_dwInactiveCaptionGradient = GetSysColor(COLOR_GRADIENTINACTIVECAPTION); // Inactive caption bar gradient
	g_dwInactiveCaptionText = GetSysColor(COLOR_ACTIVEBORDER); // Inactive caption text
	g_dwInactiveBorder = GetSysColor(COLOR_INACTIVEBORDER); // Inactive window border

	// And the window frame.
	g_dwWindowFrame = GetSysColor(COLOR_WINDOWFRAME); // Window frame color

	return;
	//GetSystemMetrics(SM_SWAPBUTTON); i will prolly have to think about this later
}