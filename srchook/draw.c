/* * * * * * * *\
	DRAW.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's drawing functions.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "ntshook.h"
#include "draw.h"
#include "oebitmap.h"
#include <strsafe.h>

/* Global Variables */
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

/* Functions */

/* * * *\
	NTStyleDrawWindow -
		Draws window using the
		DrawWindow* helper
		functions.
\* * * */
VOID NTStyleDrawWindow(_In_ HWND hWnd, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// Always refresh the colors and metrics before drawing
	NTStyleGetWindowMetrics();

	// Make sure we're only painting windows with titlebars
	if ((GetWindowLongPtr(hWnd, GWL_STYLE) & WS_SYSMENU) == WS_SYSMENU)
	{
		HDC hdc = NULL;
		HDC hdcMem = NULL;
		WINDOWINFO wi;
		// TODO: I could create an offscreen DC and
		// then blit it to the screen afterwards.

		// Verify our window handle
		if (hWnd != NULL)
			hdc = GetWindowDC(hWnd);

		if (hdc)
			hdcMem = CreateCompatibleDC(hdc);

		if (hdcMem)
		{
			HBITMAP hbm = NULL;
			HBRUSH hbr = NULL;

			RECT rc = { 0, 0, 0, 0 };

			// Get external window size
			wi.cbSize = sizeof(WINDOWINFO);
			GetWindowInfo(hWnd, &wi);

			// Setup our newly created Memory DC
			hbm = CreateCompatibleBitmap(hdc, wi.rcWindow.right - wi.rcWindow.left,
				wi.rcWindow.bottom - wi.rcWindow.top);

			SelectObject(hdcMem, hbm);

			// Setup some metrics
			g_iBorderHeight = wi.cyWindowBorders - 1;
			g_iBorderWidth = wi.cxWindowBorders - 1;

			// Draw the background for... transparency reasons
			rc.right = wi.rcWindow.right - wi.rcWindow.left;
			rc.bottom = wi.rcWindow.bottom - wi.rcWindow.top;
			hbr = GetSysColorBrush(COLOR_APPWORKSPACE);
			FillRect(hdcMem, &rc, hbr);

			// Draw the window
			// TODO: make caption return a rect for the window title
			// so that the text can center itself a little better
			NTStyleDrawWindowCaption(hdcMem, &wi, wParam, lParam);
			NTStyleDrawWindowTitle(hWnd, hdcMem, &wi, wParam, lParam);
			NTStyleDrawWindowButtons(hWnd, hdcMem, &wi, wParam, lParam);
			NTStyleDrawWindowBorders(hdcMem, &wi, wParam, lParam);

			// Blit it to our actual window
			TransparentBlt(hdc, 0, 0, wi.rcWindow.right - wi.rcWindow.left,
				wi.rcWindow.bottom - wi.rcWindow.top, hdcMem, 0, 0, 
				wi.rcWindow.right - wi.rcWindow.left,
				wi.rcWindow.bottom - wi.rcWindow.top, GetSysColor(COLOR_APPWORKSPACE)); //0xFF00FF

			// Cleanup
			if (hdcMem)
				DeleteDC(hdcMem);
			if (hbr)
				DeleteObject(hbr);
			if (hbm)
				DeleteObject(hbm);
		}

		// Cleanup
		if (hdc)
			ReleaseDC(hWnd, hdc);
	}

	return;
}

/* * * *\
	NTStyleDrawWindowBorders -
		Draws window borders
\* * * */
VOID NTStyleDrawWindowBorders(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	HBRUSH hbrInit = NULL;
	HBRUSH hbr = NULL;
	HPEN hpnInit = NULL;
	HPEN hpn = NULL;

	BOOL bIsActiveWindow = FALSE;
	BOOL bIsDlgWindow = (pwi->dwExStyle & WS_EX_DLGMODALFRAME) == WS_EX_DLGMODALFRAME;
	INT iCaptionColor = 0;
	INT iBorderColor = 0;

	UINT uWinW = 0;
	UINT uWinH = 0;

	INT i;

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

	// Check whether or not we're the active window
	// and change colors based on that
	bIsActiveWindow = pwi->dwWindowStatus == WS_ACTIVECAPTION;
	iBorderColor = bIsActiveWindow ? COLOR_ACTIVEBORDER : COLOR_INACTIVEBORDER;
	iCaptionColor = bIsActiveWindow ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION;

	// Get window width, height and border size
	uWinW = pwi->rcWindow.right - pwi->rcWindow.left;
	uWinH = pwi->rcWindow.bottom - pwi->rcWindow.top;

	// Draw the window frame
	i = 0;

	for (i = 0; i < 4; i++)
	{
		INT iModX = (i & 1) == 1;
		INT iModY = (i & 2) == 2;

		RECT rc = {
			(i == 2) * (uWinW - g_iBorderWidth - 1),
			(i == 3) * (uWinH - g_iBorderHeight - 1),
			((i == 0) ? g_iBorderWidth : uWinW) + 1,
			((i == 1) ? g_iBorderHeight : uWinH) + 1
		};

		// Draw rectangle
		hbr = GetSysColorBrush(bIsDlgWindow ? iCaptionColor : iBorderColor);
		FillRect(hDC, &rc, hbr);
	}

	// Draw the window frame's inner frame
	RECT rc = {
		g_iBorderWidth,
		g_iBorderHeight,
		uWinW - g_iBorderWidth,
		uWinH - g_iBorderHeight
	};

	hbr = GetSysColorBrush(bIsDlgWindow ? COLOR_WINDOW : COLOR_WINDOWFRAME);
	FrameRect(hDC, &rc, hbr);

	// Draw the outer frame
	rc.left = 0;
	rc.top = 0;
	rc.right = uWinW;
	rc.bottom = uWinH;

	hbr = GetSysColorBrush(COLOR_WINDOWFRAME);
	FrameRect(hDC, &rc, hbr);

	if (!bIsDlgWindow)
	{
		// Set up our brushes
		hbr = GetSysColorBrush(iBorderColor);
		hpn = CreatePen(PS_SOLID, 0, (COLORREF)GetSysColor(COLOR_WINDOWFRAME));

		hbrInit = SelectObject(hDC, hbr);
		hpnInit = SelectObject(hDC, hpn);

		// Paint the "caps"
		i = 0;

		for (i = 0; i < 4; i++)
		{
			INT iFlipX = (i & 1) == 1;
			INT iFlipY = (i & 2) == 2;
			UINT uFlipX = (iFlipX ? -1 : 1);
			UINT uFlipY = (iFlipY ? -1 : 1);

			POINT apt[7] = { 0, 0 };

			// Calculate the polygon points
			apt[0].x = 0 + uWinW * iFlipX - iFlipX;
			apt[0].y = 0 + uWinH * iFlipY - iFlipY;
			apt[1].x = apt[0].x + uFlipX * (g_iCaptionHeight + g_iBorderWidth);
			apt[1].y = apt[0].y;
			apt[2].x = apt[1].x;
			apt[2].y = apt[0].y + uFlipY * g_iBorderHeight;
			apt[3].x = apt[1].x - uFlipX * (g_iCaptionHeight);
			apt[3].y = apt[2].y;
			apt[4].x = apt[3].x;
			apt[4].y = apt[2].y + uFlipY * (g_iCaptionHeight);
			apt[5].x = apt[0].x;
			apt[5].y = apt[4].y;

			// These are necessary to close the gap
			apt[6].x = apt[0].x;
			apt[6].y = apt[0].y;

			// Draw
			Polygon(hDC, apt, sizeof(apt) / sizeof(apt[0]));
		}
	}

	// Cleanup
	if (hbrInit)
		SelectObject(hDC, hbrInit);

	if (hbr)
		DeleteObject(hbr);

	if (hpnInit)
		SelectObject(hDC, hpnInit);

	if (hpn)
		DeleteObject(hpn);

	return;
}

/* * * *\
	NTStyleDrawWindowCaption -
		Draws window caption & title
\* * * */
VOID NTStyleDrawWindowCaption(_In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	/* If an application processes the WM_NCACTIVATE message, after
	processing it must return TRUE to direct the system to complete
	the change of active window. If the window is minimized when the
	application receives the WM_NCACTIVATE message, it should pass
	the message to DefWindowProc. In such cases, the default function
	redraws the label for the icon. */

	HBRUSH hbr = NULL;

	BOOL bIsActiveWindow = FALSE;
	INT iGradientCaptionColor = 0;
	INT iCaptionColor = 0;

	RECT rc = { 0, 0, 0, 0 };
	UINT uWinW = 0;

	// Check whether or not we're the active window
	// and change colors based on that
	bIsActiveWindow = pwi->dwWindowStatus == WS_ACTIVECAPTION;
	iGradientCaptionColor = bIsActiveWindow ? COLOR_GRADIENTACTIVECAPTION : COLOR_GRADIENTINACTIVECAPTION;
	iCaptionColor = bIsActiveWindow ? COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION;

	// Get window width
	uWinW = pwi->rcWindow.right - pwi->rcWindow.left;

	// Calculate the rect points
	rc.left = g_iBorderWidth;
	rc.top = g_iBorderHeight;
	rc.right = uWinW - rc.left;
	rc.bottom = rc.top + g_iCaptionHeight + 1;

	// Draw the caption rectangle
	// TODO: do it with a gradient
	hbr = GetSysColorBrush(iCaptionColor);
	FillRect(hDC, &rc, hbr);

	// Draw the frame around it
	hbr = GetSysColorBrush(COLOR_WINDOWFRAME);
	FrameRect(hDC, &rc, hbr);

	// Clean up
	if (hbr)
		DeleteObject(hbr);

	return;
}

/* * * *\
	NTStyleDrawWindowButtons -
		Draws window buttons
\* * * */
VOID NTStyleDrawWindowButtons(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	HBRUSH hbrInit = NULL;
	HBRUSH hbr = NULL;
	HPEN hpnInit = NULL;
	HPEN hpn = NULL;

	RECT rc = { 0, 0, 0, 0 };
	RECT rcT = { 0, 0, 0, 0 };

	UINT uWinW = 0;
	UINT uTriW = 0;
	UINT uTriH = 0;

	UINT uiSysMenuSpace = (pwi->dwExStyle & WS_EX_MDICHILD) == WS_EX_MDICHILD ? 6 : 3;
	BOOL bDrawMinBox = (pwi->dwStyle & WS_MINIMIZEBOX) == WS_MINIMIZEBOX;
	BOOL bDrawMaxBox = (pwi->dwStyle & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX;

	// Get window width
	uWinW = pwi->rcWindow.right - pwi->rcWindow.left;

	// Setup our basic rectangle
	rc.top = g_iBorderHeight;
	rc.left = g_iBorderWidth;
	rc.bottom = rc.top + g_iCaptionHeight + 1;
	rc.right = rc.left + g_iCaptionHeight + 1;

	/* BEGIN SYSTEM MENU */
	// Draw background
	hbr = GetSysColorBrush(COLOR_BTNFACE);
	FillRect(hDC, &rc, hbr);

	// Draw the frame
	hbr = GetSysColorBrush(COLOR_WINDOWFRAME);
	FrameRect(hDC, &rc, hbr);

	// Draw shadow
	hbr = GetSysColorBrush(COLOR_BTNSHADOW);

	rcT.top = rc.top + (g_iCaptionHeight / 2);
	rcT.left = rc.left + uiSysMenuSpace + 1;
	rcT.bottom = rcT.top + 3;
	rcT.right = rcT.left + g_iCaptionHeight - 2 * uiSysMenuSpace;

	FillRect(hDC, &rcT, hbr);

	// Draw "minus" inside
	hbr = GetSysColorBrush(COLOR_BTNHIGHLIGHT);

	rcT.top -= 1;
	rcT.left -= 1;
	rcT.bottom -= 1;
	rcT.right -= 1;

	FillRect(hDC, &rcT, hbr);

	// Draw "minus" border
	hbr = GetSysColorBrush(COLOR_BTNTEXT);
	FrameRect(hDC, &rcT, hbr);
	/* END SYSTEM MENU */

	if (bDrawMaxBox | bDrawMinBox)
	{
		// Setup our basic rectangle
		rc.top = g_iBorderHeight;
		rc.right = uWinW - g_iBorderWidth;
		rc.bottom = rc.top + g_iCaptionHeight + 1;
		rc.left = rc.right - g_iCaptionHeight - 1;
	}

	/* BEGIN MAXIMIZEBOX */
	if (bDrawMaxBox)
	{
		WINDOWPLACEMENT wp;
		BOOL bWindowMaximized;

		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hWnd, &wp);
		bWindowMaximized = (wp.showCmd) == SW_MAXIMIZE;

		NTStyleDrawFrameControl(hDC, rc, NSDFC_TYPE_CAPTION,
			bWindowMaximized ? NSDFC_STYLE_MAXIMIZE : NSDFC_STYLE_NONE);
	}
	/* END MAXIMIZEBOX */

	/* BEGIN MINIMIZEBOX */
	if (bDrawMinBox)
	{
		if (bDrawMaxBox)
		{
			rc.left -= g_iCaptionHeight;
			rc.right -= g_iCaptionHeight;
		}

		NTStyleDrawFrameControl(hDC, rc, NSDFC_TYPE_CAPTION, NSDFC_STYLE_MINIMIZE);
	}
	/* END MINIMIZEBOX */

	// Cleanup
	if (hbrInit)
		SelectObject(hDC, hbrInit);

	if (hbr)
		DeleteObject(hbr);

	if (hpnInit)
		SelectObject(hDC, hpnInit);

	if (hpn)
		DeleteObject(hpn);

	return;
}

/* * * *\
	NTStyleDrawWindowTitle -
		Draws window title text
\* * * */
VOID NTStyleDrawWindowTitle(_In_ HWND hWnd, _In_ HDC hDC, _In_ PWINDOWINFO pwi, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	NONCLIENTMETRICS ncm;
	HBRUSH hbrInit = NULL;
	HBRUSH hbr = NULL;
	HFONT hftInit = NULL;
	HFONT hft = NULL;

	BOOL bDrawMinBox = (pwi->dwStyle & WS_MINIMIZEBOX)
		== WS_MINIMIZEBOX;
	BOOL bDrawMaxBox = (pwi->dwStyle & WS_MAXIMIZEBOX)
		== WS_MAXIMIZEBOX;
	BOOL bIsActiveWindow = FALSE;
	INT iCaptionColor = 0;
	INT iCaptionTextColor = 0;

	UINT uWinW = 0;

	RECT rc = { 0, 0, 0, 0 };
	INT cTxtLen = 0;
	LPWSTR pszTxt;

	// Check whether or not we're the active window
	// and change colors based on that
	bIsActiveWindow = pwi->dwWindowStatus == WS_ACTIVECAPTION;
	iCaptionTextColor = bIsActiveWindow ?
		COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT;
	iCaptionColor = bIsActiveWindow ?
		COLOR_ACTIVECAPTION : COLOR_INACTIVECAPTION;

	hbr = GetSysColorBrush(iCaptionColor);

	// Get the caption font
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0);

	hft = CreateFontIndirect(&ncm.lfCaptionFont);

	// Set up our HDC for the text
	SetTextColor(hDC, (COLORREF)GetSysColor(iCaptionTextColor));
	SetBkMode(hDC, TRANSPARENT);

	hbrInit = SelectObject(hDC, hbr);
	hftInit = SelectObject(hDC, hft);

	// Get our window metrics
	uWinW = pwi->rcWindow.right - pwi->rcWindow.left;

	g_iBorderHeight = pwi->cyWindowBorders - 1;
	g_iBorderWidth = pwi->cxWindowBorders - 1;

	// Calculate the rect points for the text
	rc.left = g_iBorderWidth + g_iCaptionHeight + 1;
	rc.top = g_iBorderHeight;
	rc.right = uWinW - g_iCaptionHeight * bDrawMinBox
		- g_iCaptionHeight * bDrawMaxBox;
	rc.bottom = rc.top + g_iCaptionHeight;

	// Allocate memory for the caption text
	cTxtLen = GetWindowTextLength(hWnd);

	pszTxt = (LPWSTR)VirtualAlloc((LPVOID)NULL,
		(SIZE_T)cTxtLen + 1, MEM_COMMIT,
		PAGE_READWRITE);

	// TODO: draw pretty text and not the ugly
	// stuff, actually use some good font funcs

	if (pszTxt)
	{
		// Get the caption text
		GetWindowText(hWnd, pszTxt, cTxtLen + 1);

		// Draw the caption text
		DrawText(hDC, pszTxt, cTxtLen + 1, &rc, DT_CENTER
			| DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);

		// Free the memory
		VirtualFree(pszTxt, 0, MEM_RELEASE);
	}

	// Cleanup
	if (hftInit)
		SelectObject(hDC, hftInit);
	
	if (hft)
		DeleteObject(hft);

	if (hbrInit)
		SelectObject(hDC, hbrInit);

	if (hbr)
		DeleteObject(hbr);

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
	g_iDlgFrmWidth = GetSystemMetrics(SM_CXDLGFRAME); // Dialog frame width
	g_iDlgFrmHeight = GetSystemMetrics(SM_CYDLGFRAME); // Dialog frame height

	// And the window frame.
	g_dwWindowFrame = GetSysColor(COLOR_WINDOWFRAME); // Window frame color

	return;
	//GetSystemMetrics(SM_SWAPBUTTON); i will prolly have to think about this later

	/*poop coode pooop code code for pooping
	big chungus big chungus
	pass the boof
	gimme sum
	if [???] is dirty poop then(succubus train wreck )*/
}

/* * * *\
	NTStyleDrawFrameControl -
		Draws the control.
	RETURNS -
		This function returns
		TRUE if successful
		and FALSE otherwise
\* * * */
BOOL NTStyleDrawFrameControl(_In_ HDC hDC, _In_ RECT rc, _In_ UINT uType, _In_ UINT uState)
{
	// States and types:
	// https://devblogs.microsoft.com/oldnewthing/20050802-13/?p=34743
	// https://reactos.org/wiki/Techwiki:Win32k/SERVERINFO
	
	// Colors:
	// COLOR_BTNTEXT
	// COLOR_BTNFACE
	// COLOR_BTNSHADOW
	// COLOR_BTNHIGHLIGHT
	switch (uType)
	{
	case (NSDFC_TYPE_CAPTION):
	{
		WCHAR pszGlyphMax[2] = TEXT("t"); // t/1
		WCHAR pszGlyphMin[2] = TEXT("u"); // u/0
		WCHAR pszGlyphMaxed[2] = TEXT("v"); // v/2
		PWCHAR pszGlyph = pszGlyphMax;
		HBRUSH hbr = NULL;
		HFONT hftInit = NULL;
		HFONT hft = NULL;
		RECT rcT = { rc.left + 1, rc.top + 1,
			rc.right - 1, rc.bottom - 1 };

		// Draw the highlight
		DrawFrameControl(hDC, &rcT, DFC_BUTTON, DFCS_BUTTONPUSH);

		// Draw the frame
		hbr = GetSysColorBrush(COLOR_WINDOWFRAME);
		FrameRect(hDC, &rc, hbr);

		// Get our caption font
		hft = CreateFont(g_iCaptionHeight - (g_iCaptionHeight / 3), 0, 0, 0, 0,
			FALSE, FALSE, FALSE, SYMBOL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, L"Marlett");
		hftInit = SelectObject(hDC, hft);

		if (uState == NSDFC_STYLE_MINIMIZE)
			pszGlyph = pszGlyphMin;
		else if (uState == NSDFC_STYLE_MAXIMIZE)
			pszGlyph = pszGlyphMaxed;

		// Set up our HDC for the text
		SetTextColor(hDC, (COLORREF)GetSysColor(COLOR_BTNTEXT));
		SetBkMode(hDC, TRANSPARENT);

		// Draw the arrow
		DrawText(hDC, pszGlyph, 1, &rcT, DT_CENTER
			| DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);

		// Cleanup
		if (hftInit)
			SelectObject(hDC, hftInit);

		if (hft)
			DeleteObject(hft);

		if (hbr)
			DeleteObject(hbr);

		break;
	}

	case (NSDFC_TYPE_BUTTON):
	{
		break;
	}

	default:
		return FALSE;
		break;
	}

	return TRUE;
}
