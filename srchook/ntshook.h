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
// Hook Procedure
__declspec(dllexport) LRESULT NTStyleHookProc(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

// Drawing Functions
VOID NTStyleDrawWindowBorders();
VOID NTStyleDrawWindowCaption();
