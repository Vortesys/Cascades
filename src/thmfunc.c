/* * * * * * * *\
	THMFUNC.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Theme functions for passing to RegisterUserApiHook.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include <wtypes.h>

/* Functions */
BOOL WINAPI RealAdjustWindowRectEx(LPRECT, DWORD, BOOL, DWORD);
BOOL WINAPI RealGetScrollInfo(HWND, INT, LPSCROLLINFO);
INT WINAPI RealSetScrollInfo(HWND, int, LPCSCROLLINFO, BOOL);
// ThemeEnableScrollInfoProc
// ThemeSetWindowRgn
LRESULT WINAPI RealDefWindowProcW(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI RealDefWindowProcA(HWND, UINT, WPARAM, LPARAM);
// ThemePreWndProc
// ThemePostWndProc
// GetOwpMsgMask
// ThemePreDefDlgProc
// ThemePostDefDlgProc
// GetDdpMsgMask
int WINAPI RealGetSystemMetrics(int);
BOOL WINAPI RealSystemParametersInfoA(UINT, UINT, PVOID, UINT);
BOOL WINAPI RealSystemParametersInfoW(UINT, UINT, PVOID, UINT);
BOOL WINAPI RealDrawFrameControl(HDC, LPRECT, UINT, UINT);
BOOL WINAPI RealUserDrawCaption(HWND hWnd, HDC hDC, LPCRECT lpRc, UINT uFlags);
BOOL WINAPI RealMDIRedrawFrame(HWND, DWORD);

LRESULT ThemeDefWindowProcA(
	[in] HWND   hWnd,
	[in] UINT   Msg,
	[in] WPARAM wParam,
	[in] LPARAM lParam
);
LRESULT ThemeDefWindowProcW(
	[in] HWND   hWnd,
	[in] UINT   Msg,
	[in] WPARAM wParam,
	[in] LPARAM lParam
);
UAHOWP		ThemeDefWndProcArray;
BOOL ThemeGetScrollInfo(
	[in]      HWND         hwnd,
	[in]      int          nBar,
	[in, out] LPSCROLLINFO lpsi
);
int ThemeSetScrollInfo(
	[in] HWND          hwnd,
	[in] int           nBar,
	[in] LPCSCROLLINFO lpsi,
	[in] BOOL          redraw
);
BOOL ThemeEnableScrollBar(
	[in] HWND hWnd,
	[in] UINT wSBflags,
	[in] UINT wArrows
);
BOOL ThemeAdjustWindowRectEx(
	[in, out] LPRECT lpRect,
	[in]      DWORD  dwStyle,
	[in]      BOOL   bMenu,
	[in]      DWORD  dwExStyle
);
int ThemeSetWindowRgn(
	[in] HWND hWnd,
	[in] HRGN hRgn,
	[in] BOOL bRedraw
);

static LRESULT CALLBACK ThemeDlgPreWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown)
{
	return 0;
}

static LRESULT CALLBACK ThemePostWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown)
{
	return 0;
}
UAHOWP		WndProcArray;
WNDPROC_OWP	PreDefDlgProc;
WNDPROC_OWP	PostDefDlgProc;
UAHOWP		DlgProcArray;
int ThemeGetSystemMetrics(
	[in] int nIndex
);
BOOL SystemParametersInfoA(
	[in]      UINT  uiAction,
	[in]      UINT  uiParam,
	[in, out] PVOID pvParam,
	[in]      UINT  fWinIni
);
BOOL ThemeSystemParametersInfoW(
	[in]      UINT  uiAction,
	[in]      UINT  uiParam,
	[in, out] PVOID pvParam,
	[in]      UINT  fWinIni
);
FARPROC		ThemeForceResetUserApiHook;
BOOL ThemeDrawFrameControl(
	[in] HDC    hdc,
	[in] LPRECT lprc,
	[in] UINT   uType,
	[in] UINT   uState
);
BOOL ThemeDrawCaption(
	[in] HWND       hwnd,
	[in] HDC        hdc,
	[in] const RECT* lprect,
	[in] UINT       flags
);
FARPROC		ThemeMDIRedrawFrame;
FARPROC		ThemeGetRealWindowOwner;