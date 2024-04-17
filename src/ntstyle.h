/* * * * * * * *\
	NTSTYLE.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's primary header file.
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
extern HHOOK		g_hhkNTShk32;
extern HHOOK		g_hhkNTShk64;
// Strings
extern WCHAR		g_szAppTitle[64];
// Other
extern BOOL			g_bSystem64;

/* Function Prototypes */
// NTSTYLE.C
BOOL CALLBACK NtStyleEnumWindowProc(_In_ HWND hwnd, _In_ LPARAM lParam);
DWORD NtStyleCreateHook(_In_ HINSTANCE hInst, _In_ LPWSTR lpNTStyleHook, _In_ BOOL bDisableTheming, _Out_ HHOOK hhkNTShk);
// DLGPROC.C
INT_PTR CALLBACK NtStyleDialogProc(_In_ HWND hDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
