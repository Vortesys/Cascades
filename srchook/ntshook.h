/* * * * * * * *\
	NTSTYLE.H -
		Copyright © 2023 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's primary header file.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once

/* Includes */
#include <wtypes.h>

/* Global Variables */

/* Function Prototypes */
__declspec(dllexport) LRESULT NTStyleHookProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
