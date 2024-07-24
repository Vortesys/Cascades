/* * * * * * * *\
	THMFUNC.H -
		Copyright © 2024 Brady McDermott
	DESCRIPTION -
		Functions for use with the
		User32 Api Hook.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once

/* Includes */
#include <wtypes.h>

/* Functions */
LRESULT CALLBACK ThemeDefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ThemeDefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ThemePreWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
LRESULT CALLBACK ThemePostWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
LRESULT CALLBACK ThemePreDefDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
LRESULT CALLBACK ThemePostDefDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
int WINAPI ThemeSetWindowRgn(HWND hWnd, HRGN hRgn, BOOL bRedraw);
BOOL WINAPI ThemeGetScrollInfo(HWND hWnd, int nBar, LPSCROLLINFO lpsi);
int WINAPI ThemeSetScrollInfo(HWND hWnd, int nBar, LPCSCROLLINFO lpsi, BOOL bRedraw);
