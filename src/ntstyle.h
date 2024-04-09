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
// #pragma comment(lib, "ComCtl32.lib")

/* Includes */
#include <wtypes.h>

/* Global Variables */
// Handles
extern HINSTANCE	g_hAppInstance;

/* Function Prototypes */
BOOL CALLBACK NTStyleEnumWindowProc(_In_ HWND hwnd, _In_ LPARAM lParam);
