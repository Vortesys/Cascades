/* * * * * * * *\
	GUI.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Cascades UI main file.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "gui.h"
#include "cfg.h"
#include "resource.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <CommCtrl.h>

/* Variables */
// Handles
HINSTANCE	g_hAppInstance = NULL;
HMODULE		g_hDllInstance = NULL;
// Strings
WCHAR		g_szAppTitle[64];

/* Functions */

/* * * *\
	GuiMain -
		GUI Main entry point and message loop.
\* * * */
int WINAPI GuiMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	MSG msg = { 0 };
	HWND hDlg;

	// Get our own hInstance and save it for later
	g_hAppInstance = hInstance;

	// Create our main window dialog
	InitCommonControls();
	hDlg = CreateDialogParam(g_hAppInstance, MAKEINTRESOURCE(IDD_MAIN), 0, CascadesDialogProc, 0);
	ShowWindow(hDlg, nCmdShow);

	// Enter the dialog message loop.
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!IsDialogMessage(hDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

/* * * *\
	CascadesToggleHook -
		NT Style's hook creation and removal function.
\* * * */
BOOL CascadesToggleHook(BOOL bInstall)
{
	if (bInstall)
		return DoStartSvc();
	else
		return DoStopSvc();
}
