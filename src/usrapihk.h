/* * * * * * * *\
	USRAPIHK.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		User32 UserApiHook function prototypes. Info
		and structures sourced primarily from the ReactOS
		wiki, source code and Win32ss (Shane Forunier).
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once

/* Definitions */
#define WM_UAHINIT			0x0000031b
#define WM_UAHDRAWMENU		0x00000091
#define WM_UAHDRAWITEM		0x00000092 // WM_DRAWITEM
#define WM_UAHINITMENU		0x00000093
#define WM_UAHMEASUREITEM	0x00000094 // WM_MEASUREITEM
#define WM_UAHDRAWMENUNC	0x00000095
#define WM_NCUAHDRAWCAPTION	0x000000AE
#define WM_NCUAHDRAWFRAME	0x000000AF
#define UAH_HOOK_MESSAGE(uahowp, msg) uahowp.MsgBitArray[msg/8] |= (1 << (msg % 8));
#define UAH_IS_MESSAGE_HOOKED(uahowp, msg) (uahowp.MsgBitArray[msg/8] & (1 << (msg % 8)))
#define UAHOWP_MAX_SIZE WM_USER/8

/* Structures */

// NT 5.1
typedef struct _USERAPIHOOKINFO_XP
{
	HINSTANCE hInstance; // hInstance
	FARPROC CallbackFunc; // Callback function
} USERAPIHOOKINFO_XP, * PUSERAPIHOOKINFO_XP;
typedef BOOL(WINAPI* RUAH_XP)(PUSERAPIHOOKINFO_XP);

// NT 5.2 and newer
typedef struct _USERAPIHOOKINFO
{
	DWORD m_size; // Size of the structure
	LPCWSTR m_dllname1; // DLL 1
	LPCWSTR m_funname1; // Callback function
	LPCWSTR m_dllname2; // DLL 2
	LPCWSTR m_funname2; // Callback function
} USERAPIHOOKINFO, * PUSERAPIHOOKINFO;
typedef BOOL(WINAPI* RUAH)(PUSERAPIHOOKINFO);

typedef LRESULT(CALLBACK* WNDPROC_OWP)(HWND, UINT, WPARAM, LPARAM, ULONG_PTR, PDWORD);

typedef struct _UAHOWP
{
	BYTE* MsgBitArray;
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
BOOL WINAPI UnregisterUserApiHook(VOID);
