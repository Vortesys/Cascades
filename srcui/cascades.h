/* * * * * * * *\
	CASCADES.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Primary header file.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "ComCtl32.lib")

/* Includes */
#include <wtypes.h>

/* Global Variables */
// Handles
extern HINSTANCE	g_hAppInstance;
// Strings
extern WCHAR		g_szAppTitle[64];
// Other
extern BOOL			g_bSystem64;

/* Function Prototypes */
// NTSTYLE.C
BOOL CALLBACK NtStyleEnumWindowProc(_In_ HWND hwnd, _In_ LPARAM lParam);
BOOL NtStyleToggleHook(BOOL bInstall);

// DLGPROC.C
INT_PTR CALLBACK NtStyleDialogProc(_In_ HWND hDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
