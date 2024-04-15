/* * * * * * * *\
	DLGPROC.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		NT Style's primary dialog procedures.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
 \* * * * * * * */

/* Headers */
#include "ntstyle.h"
#include "resource.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

/* Functions */

/* * * *\
	NTStyleDialogProc -
		NT Style's dialog procedure.
\* * * */
INT_PTR CALLBACK NTStyleDialogProc(
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
			DWORD dwLastError = 0;

			if (g_bSystem64)
				dwLastError = NTStyleCreateHook(g_hAppInstance, L"ntshk64.dll",
					IsDlgButtonChecked(hDlg, IDC_THEMEOFF), g_hhkNTShk64);

			// load that 32 bit ish
			dwLastError = NTStyleCreateHook(g_hAppInstance, L"ntshk32.dll",
				IsDlgButtonChecked(hDlg, IDC_THEMEOFF), g_hhkNTShk32);

			if (g_hhkNTShk32 || g_hhkNTShk64)
			{
				// Start NT Style
				if (!g_hhkNTShk32)
					MessageBox(hDlg, L"Started NT Style.", L"NT Style (AMD64)",
						MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
				else if (!g_hhkNTShk64)
					MessageBox(hDlg, L"Started NT Style.", L"NT Style (IA32)",
						MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
				else
					MessageBox(hDlg, L"Started NT Style.", L"NT Style (AMD64 + WOW)",
						MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
			}

			Button_Enable(GetDlgItem(hDlg, IDC_START), FALSE);
			Button_Enable(GetDlgItem(hDlg, IDC_STOP), TRUE);

			return 0;
		}

		case IDC_STOP:
			if (g_hhkNTShk32)
				UnhookWindowsHookEx(g_hhkNTShk32);
			if (g_hhkNTShk64)
				UnhookWindowsHookEx(g_hhkNTShk64);

			Button_Enable(GetDlgItem(hDlg, IDC_START), TRUE);
			Button_Enable(GetDlgItem(hDlg, IDC_STOP), FALSE);

			return 0;
		}
		break;

	case WM_INITDIALOG:
		Button_SetCheck(GetDlgItem(hDlg, IDC_THEMEOFF), BST_CHECKED);
		Button_Enable(GetDlgItem(hDlg, IDC_START), TRUE);
		Button_Enable(GetDlgItem(hDlg, IDC_STOP), FALSE);
		return TRUE;

	case WM_CLOSE:
		if (MessageBox(hDlg, L"Quit NT Style?", L"Close",
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