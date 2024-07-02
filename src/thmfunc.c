/* * * * * * * *\
	THMFUNC.C -
		Copyright © 2024 Brady McDermott
	DESCRIPTION -
		Functions for use with the
		User32 Api Hook.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Includes */
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "usrapihk.h"
#include "main.h"
#include "thmfunc.h"

/* Functions */

/* * * *\
	ThemeDefWindowProcA -
		Stub.
\* * * */
LRESULT CALLBACK ThemeDefWindowProcA(
	HWND   hWnd,
	UINT   Msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	return g_user32ApiHook.DefWindowProcA(hWnd, Msg, wParam, lParam);
}

/* * * *\
	ThemeDefWindowProcW -
		Default ApiHook window procedure.
\* * * */
LRESULT CALLBACK ThemeDefWindowProcW(
	HWND   hWnd,
	UINT   Msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (Msg)
	{

	case WM_NCPAINT:
		return ThemeHandleNCPaint(hWnd, (HRGN)wParam);
		//
		// WM_NCUAHDRAWCAPTION : wParam are DC_* flags.
		//
	case WM_NCUAHDRAWCAPTION:
		//
		// WM_NCUAHDRAWFRAME : wParam is HDC, lParam are DC_ACTIVE and or DC_REDRAWHUNGWND.
		//
	case WM_NCUAHDRAWFRAME:
	case WM_NCACTIVATE:
		// Determine whether the window is active or not
		// Determine whether or not the window has a caption bar
		// Determine whether or not the window is visible or not
		// If window is minimized, draw the icon
		if ((GetWindowLongW(hWnd, GWL_STYLE) & WS_CAPTION) != WS_CAPTION)
			return TRUE;

		ThemeHandleNCPaint(hWnd, (HRGN)1);
		return TRUE;

	case WM_NCRBUTTONDOWN:
	case WM_NCMBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCLBUTTONDBLCLK:
	case WM_NCMOUSEMOVE:
	{
		ThemeHandleNcMouseMove(hWnd, Msg, wParam, lParam);
		break;
	}
	//case WM_NCHITTEST:
		//return DefWndNCHitTest(hWnd, Point);
	//case WM_SYSCOMMAND:
		// SC_VSCROLL/SC_HSCROLL deal with them later
	//case WM_CREATE:
		//MessageBox(NULL, L"Hurrah! WM_CREATE!", L"Cascades", MB_OK);
		//OutputDebugString(L"Hurrah! WM_CREATE!");
		//break;
	default:
		return g_user32ApiHook.DefWindowProcW(hWnd, Msg, wParam, lParam);
	}
}

/* * * *\
	ThemePreWindowProc -
		Stub.
\* * * */
LRESULT CALLBACK ThemePreWindowProc(
	HWND   hWnd,
	UINT   Msg,
	WPARAM wParam,
	LPARAM lParam,
	ULONG_PTR ret,
	PDWORD unknown
)
{
	return 0;
}

/* * * *\
	ThemePostWindowProc -
		Stub.
\* * * */
LRESULT CALLBACK ThemePostWindowProc(
	HWND   hWnd,
	UINT   Msg,
	WPARAM wParam,
	LPARAM lParam,
	ULONG_PTR ret,
	PDWORD unknown
)
{
	return 0;
}

/* * * *\
	ThemePreDefDlgProc -
		Stub.
\* * * */
LRESULT CALLBACK ThemePreDefDlgProc(
	HWND   hWnd,
	UINT   Msg,
	WPARAM wParam,
	LPARAM lParam,
	ULONG_PTR ret,
	PDWORD unknown
)
{
	return 0;
}

/* * * *\
	ThemePostDefDlgProc -
		Stub.
\* * * */
LRESULT CALLBACK ThemePostDefDlgProc(
	HWND   hWnd,
	UINT   Msg,
	WPARAM wParam,
	LPARAM lParam,
	ULONG_PTR ret,
	PDWORD unknown
)
{
	return 0;
}

/* * * *\
	ThemeSetWindowRgn -
		Stub.
\* * * */
int WINAPI ThemeSetWindowRgn(
	HWND hWnd,
	HRGN hRgn,
	BOOL bRedraw
)
{
	return (int)g_user32ApiHook.SetWindowRgn(hWnd, hRgn, bRedraw);
}

/* * * *\
	ThemeGetScrollInfo -
		Stub.
\* * * */
BOOL WINAPI ThemeGetScrollInfo(
	HWND hWnd,
	int nBar,
	LPSCROLLINFO lpsi
)
{
	return (BOOL)g_user32ApiHook.GetScrollInfo(hWnd, nBar, lpsi);
}

/* * * *\
	ThemeSetScrollInfo -
		Stub.
\* * * */
int WINAPI ThemeSetScrollInfo(
	HWND hWnd,
	int nBar,
	LPCSCROLLINFO lpsi,
	BOOL bRedraw
)
{
	return (int)g_user32ApiHook.SetScrollInfo(hWnd, nBar, lpsi, bRedraw);
}
