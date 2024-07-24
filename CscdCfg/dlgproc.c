/* * * * * * * *\
	DLGPROC.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Primary dialog procedures for Cascades UI.
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
#include <shlwapi.h>

/* Functions */

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
