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
static INT g_iCaptionHeight;
static INT g_iBorderWidth;
static INT g_iBorderHeight;
static INT g_iDlgFrmWidth;
static INT g_iDlgFrmHeight;

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
		case WM_NCCREATE:
		case WM_NCCALCSIZE:
		case WM_NCACTIVATE:
		case WM_NCPAINT:
			if ((GetWindowLongPtr(pcwps->hwnd, GWL_STYLE) & WS_CAPTION) == WS_CAPTION)
			{
				NTStyleDrawWindowCaption(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
				NTStyleDrawWindowBorders(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
			}
			//return 0;
			break;

		// WM_NCDESTROY

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
	HBRUSH hbr = NULL;
	HPEN hpn = NULL;

	POINT apt[7] = { 0, 0 };
	INT asz[] = { 6, 4, 6, 4, 6, 4, 6, 4 };
	INT csz = 8;

	WINDOWINFO wi;
	BOOL bIsActiveWindow = FALSE;
	INT iGradientCaptionColor = 0;
	INT iCaptionColor = 0;
	INT iBorderColor = 0;

	RECT rc = { 0, 0, 8, 8 };
	UINT uiw = 0;
	UINT uih = 0;

	// Verify our window handle
	if (hWnd != NULL)
		hdc = GetWindowDC(hWnd);

	// Verify the device context
	if (hdc)
	{

		// Check whether or not we're the active window
		// and change colors based on that
		bIsActiveWindow = GetForegroundWindow() == hWnd;
		iGradientCaptionColor = bIsActiveWindow ? COLOR_GRADIENTACTIVECAPTION : COLOR_GRADIENTINACTIVECAPTION;
		iCaptionColor = bIsActiveWindow ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION;
		iBorderColor = bIsActiveWindow ? COLOR_ACTIVEBORDER : COLOR_INACTIVEBORDER;

		// Set up our brushes
		hbr = GetSysColorBrush(iBorderColor);
		hpn = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

		// FillRect(hdc, &rc, hbr);

		// Get external window size
		wi.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(hWnd, &wi);

		// Get window width and height
		uiw = wi.rcWindow.right - wi.rcWindow.left;
		uih = wi.rcWindow.bottom - wi.rcWindow.top;

		// Always refresh the colors and metrics before drawing
		NTStyleGetWindowMetrics();

		g_iBorderHeight = wi.cyWindowBorders;
		g_iBorderWidth = wi.cxWindowBorders;

		// Calculate the polygon points
		apt[0].x = 0;
		apt[0].y = 0;
		apt[1].x = g_iCaptionHeight + g_iBorderWidth;
		apt[1].y = 0;
		apt[2].x = g_iCaptionHeight + g_iBorderWidth;
		apt[2].y = g_iBorderHeight;
		apt[3].x = g_iBorderHeight;
		apt[3].y = g_iBorderHeight;
		apt[4].x = g_iBorderHeight;
		apt[4].y = g_iCaptionHeight + g_iBorderHeight;
		apt[5].x = 0;
		apt[5].y = g_iCaptionHeight + g_iBorderHeight;
		apt[6].x = 0;

		// Set the polygon fill mode
		SetPolyFillMode(hdc, ALTERNATE);

		// Draw
		Polygon(hdc, apt, sizeof(apt) / sizeof(apt[0]));

		// Cleanup
		if (hbr)
			DeleteObject(hbr);
		if (hpn)
			DeleteObject(hpn);

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
	g_iCaptionHeight = GetSystemMetrics(SM_CYCAPTION); // Caption bar height
	g_iBorderWidth = GetSystemMetrics(SM_CXBORDER); // Border width
	g_iBorderHeight = GetSystemMetrics(SM_CYBORDER); // Border height
	g_iDlgFrmWidth = GetSystemMetrics(SM_CXDLGFRAME); // Dialog frame width
	g_iDlgFrmHeight = GetSystemMetrics(SM_CYDLGFRAME); // Dialog frame height

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