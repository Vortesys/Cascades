/* * * * * * * *\
	USRAPIHK.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		User32 UserApiHook function prototypes. Info
		sourced primarily from the ReactOS wiki and
		Mozilla's bug tracker.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once

/* Includes */
#include <wtypes.h>

/* Structures */
typedef struct _USERAPIHOOKINFO
{
	DWORD	m_size;
	LPCWSTR	m_dllname1; // DLL 1
	LPCWSTR	m_funname1; // Callback function
	LPCWSTR	m_dllname2; // DLL 2
	LPCWSTR	m_funname2; // Callback function
} USERAPIHOOKINFO, * PUSERAPIHOOKINFO;

typedef LRESULT(CALLBACK* WNDPROC_OWP)(HWND, UINT, WPARAM, LPARAM, ULONG_PTR, PDWORD);

typedef struct _UAHOWP
{
	BYTE*	MsgBitArray;
	DWORD	Size;
} UAHOWP, * PUAHOWP;

typedef struct tagUSERAPIHOOK
{
	DWORD		size;
	WNDPROC		DefWindowProcA;
	WNDPROC		DefWindowProcW;
	UAHOWP		DefWndProcArray;
	FARPROC		GetScrollInfo;
	FARPROC		SetScrollInfo;
	FARPROC		EnableScrollBar;
	FARPROC		AdjustWindowRectEx;
	FARPROC		SetWindowRgn;
	WNDPROC_OWP	PreWndProc;
	WNDPROC_OWP	PostWndProc;
	UAHOWP		WndProcArray;
	WNDPROC_OWP	PreDefDlgProc;
	WNDPROC_OWP	PostDefDlgProc;
	UAHOWP		DlgProcArray;
	FARPROC		GetSystemMetrics;
	FARPROC		SystemParametersInfoA;
	FARPROC		SystemParametersInfoW;
	FARPROC		ForceResetUserApiHook;
	FARPROC		DrawFrameControl;
	FARPROC		DrawCaption;
	FARPROC		MDIRedrawFrame;
	FARPROC		GetRealWindowOwner;
} USERAPIHOOK, * PUSERAPIHOOK;

typedef enum _UAPIHK
{
	uahLoadInit,
	uahStop,
	uahShutdown
} UAPIHK, * PUAPIHK;

/* Function Prototypes */
BOOL WINAPI RegisterUserApiHook(PUSERAPIHOOKINFO ApiHookInfo);
DWORD CALLBACK UserApiHookProc(UAPIHK State, PUSERAPIHOOK Info);
