/* * * * * * * *\
	GUI.H -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Cascades Configuration GUI component
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "Shlwapi.lib")

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
int WINAPI GuiMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
BOOL CascadesToggleHook(BOOL bInstall);
INT_PTR CALLBACK CascadesDialogProc(_In_ HWND hDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
