/* * * * * * * *\
	CSCDHOOK.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Hook portion's primary header file.
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
#include "..\common\usrapihk.h"

/* Definitions */
#define STATUS_UNSUCCESSFUL 0xC0000001

/* Global Variables */

/* Function Prototypes */
__declspec(dllexport) VOID APIENTRY NtStyleDisableWindowTheme(_In_ HWND hWnd);

// Hook Functions
__declspec(dllexport) BOOL CALLBACK NtStyleInstallUserHook();
__declspec(dllexport) BOOL CALLBACK NtStyleInitUserHook(UAPIHK State, PUSERAPIHOOK puah);
__declspec(dllexport) BOOL CALLBACK NtStyleRemoveUserHook();

// Window Management
static LRESULT CALLBACK NtStyleDefWindowProcA(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
static LRESULT CALLBACK NtStyleDefWindowProcW(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
static LRESULT CALLBACK NtStylePreWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
static LRESULT CALLBACK NtStylePostWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
static LRESULT CALLBACK NtStyleDlgPreWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
static LRESULT CALLBACK NtStyleDlgPostWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, ULONG_PTR ret, PDWORD unknown);
static LRESULT CALLBACK NtStyleSetWindowRgn(HWND hWnd, HRGN hRgn, BOOL bRedraw);
static LRESULT CALLBACK NtStyleGetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi);
static LRESULT CALLBACK NtStyleSetScrollInfo(HWND hWnd, int fnBar, LPCSCROLLINFO lpsi, BOOL bRedraw);
LRESULT CALLBACK NtStyleWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, WNDPROC DefWndProc);

// Delay Load Functions
HRESULT DwmGetWindowAttributeDelay(HWND hwnd, DWORD dwAttribute, _In_ LPCVOID pvAttribute, DWORD cbAttribute);
HRESULT DwmSetWindowAttributeDelay(HWND hwnd, DWORD dwAttribute, _In_ LPCVOID pvAttribute, DWORD cbAttribute);
HRESULT SetWindowThemeDelay(_In_ HWND hwnd, _In_ LPCWSTR pszSubAppName, _In_ LPCWSTR pszSubIdList);
BOOL WINAPI RegisterUserApiHook(PUSERAPIHOOKINFO ApiHookInfo);
BOOL WINAPI RegisterUserApiHookWin32(HINSTANCE hInstance, FARPROC pfUserHook);
BOOL WINAPI UnregisterUserApiHook(VOID);
