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
#include <windowsx.h>
#include <CommCtrl.h>
#include <shlwapi.h>

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
	CascadesDialogProc -
		Main dialog procedure.
\* * * */
INT_PTR CALLBACK CascadesDialogProc(
	_In_ HWND hDlg,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;

		case IDC_THEMES:
			return 0;

		case IDC_START:
		{
			CascadesToggleHook(TRUE);
			/*
			if (!CascadesToggleHook(TRUE))
			{
				MessageBox(hDlg, L"Started Cascades.", L"Cascades",
					MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
			}
			else
			{
				MessageBox(hDlg, L"Failed to start Cascades.", L"Cascades",
					MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
			}
			*/
			return 0;
		}

		case IDC_STOP:
			CascadesToggleHook(FALSE);
			/*
			if (!CascadesToggleHook(FALSE))
			{
				MessageBox(hDlg, L"Failed to stop Cascades.", L"Cascades",
					MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
			}
			*/
			return 0;

		case IDC_INSTALL:
		{
			WCHAR szCurrentPath[MAX_PATH];

			// Get current filename and path
			GetModuleFileName(NULL, szCurrentPath, MAX_PATH);

			// Strip the filename from the path
			PathRemoveFileSpec(szCurrentPath);

			// Call the service and install
			ShellExecute(NULL, TEXT("runas"), TEXT("CscdSvc.exe"), TEXT("install"), szCurrentPath, SW_SHOWDEFAULT);
			return 0;
		}
		case IDC_ENABLE:
			DoEnableSvc();
			return 0;
		case IDC_DISABLE:
			DoDisableSvc();
			return 0;

		}

		break;

	case WM_INITDIALOG:
		Button_SetCheck(GetDlgItem(hDlg, IDC_INSTALL), BST_CHECKED);
		Button_Enable(GetDlgItem(hDlg, IDC_START), TRUE);
		Button_Enable(GetDlgItem(hDlg, IDC_STOP), TRUE);
		Button_Enable(GetDlgItem(hDlg, IDC_INSTALL), TRUE);
		Button_Enable(GetDlgItem(hDlg, IDC_ENABLE), TRUE);
		Button_Enable(GetDlgItem(hDlg, IDC_DISABLE), TRUE);
		return TRUE;

	case WM_CLOSE:
		if (MessageBox(hDlg, L"Quit Cascades?", L"Close",
			MB_ICONQUESTION | MB_YESNO) == IDYES)
			DestroyWindow(hDlg);
		return TRUE;

	case WM_DESTROY:
		SendMessage(hDlg, WM_COMMAND, IDC_STOP, 0);
		PostQuitMessage(0);
		return TRUE;
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
