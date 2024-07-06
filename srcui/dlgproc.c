/* * * * * * * *\
	DLGPROC.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Primary dialog procedures for Cascades UI.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "cscdui.h"
#include "resource.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

/* Functions */

/* * * *\
	CascadesDialogProc -
		Main dialog procedure.
\* * * */
INT_PTR CALLBACK NtStyleDialogProc(
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
			if (!NtStyleToggleHook(TRUE))
			{
				MessageBox(hDlg, L"Started Cascades.", L"Cascades",
					MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
			}
			else
			{
				MessageBox(hDlg, L"Failed to start Cascades.", L"Cascades",
					MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
			}

			return 0;
		}

		case IDC_STOP:
			if (!NtStyleToggleHook(FALSE))
			{
				MessageBox(hDlg, L"Failed to stop Cascades.", L"Cascades",
					MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
			}

			return 0;
		}
		break;

	case WM_INITDIALOG:
		Button_SetCheck(GetDlgItem(hDlg, IDC_THEMEOFF), BST_CHECKED);
		Button_Enable(GetDlgItem(hDlg, IDC_START), TRUE);
		Button_Enable(GetDlgItem(hDlg, IDC_STOP), TRUE);
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
