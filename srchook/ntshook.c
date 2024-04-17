/* * * * * * * *\
	NTSHOOK.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Defines the entry point for the DLL application.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
 \* * * * * * * */

/* Headers */
#include "ntshook.h"
#include "ntsdrfn.h"
#include "resource.h"
#include <dwmapi.h>
#include <uxtheme.h>
#include <shlwapi.h>

/* Global Variables */
static INT g_iSystemHasDWM = 2;
static INT g_iLockUpdateCount;

// Handles
HINSTANCE	g_hDllInstance;

/* * * *\
	DllMain -
		NT Style Hook's entry point
\* * * */
BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

/* * * *\
	NtStyleHookProc -
		NT Style Hook procedure
		Uses WH_CALLWNDPROC.
\* * * */
__declspec(dllexport) LRESULT APIENTRY NtStyleHookProc(
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	PCWPSTRUCT pcwps;

	// Switch case for window painting
	switch (uMsg)
	{
	/* BEGIN HC_ACTION */
	case HC_ACTION:
		pcwps = (CWPSTRUCT*)lParam;

		switch (pcwps->message)
		{
		// Collision Messages
			/*
		case WM_NCLBUTTONUP:
			break;
			
		case WM_NCLBUTTONDBLCLK:
			break;
			
		case WM_NCRBUTTONUP:
			break;
			
		case WM_NCHITTEST:
			break;*/

		default:
			break;
		}
	/* END HC_ACTION */

	default:
		break;
	}

	return CallNextHookEx(NULL, uMsg, wParam, lParam);
}

/* * * *\
	NtStyleDisableWindowTheme -
		de-themify that window
\* * * */
__declspec(dllexport) VOID APIENTRY NtStyleDisableWindowTheme(_In_ HWND hWnd)
{
	enum DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;

	HANDLE hProcess = NULL;
	DWORD dwProcessID = 0;
	INT cchProcessName = MAX_PATH;
	WCHAR pszProcessName[MAX_PATH] = L"";

	// DO NOT DO ANY OF THIS TO EXPLORER!!!
	// EXPLORER NEEDS DWM!!!
	if (GetWindowThreadProcessId(hWnd, &dwProcessID) == 0)
		return;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);

	if (QueryFullProcessImageName(hProcess, 0, pszProcessName, &cchProcessName))
	{
		if (StrStrI(pszProcessName, (LPWSTR)L"\\explorer.exe") == NULL)
		{
			// Nuke DWM
			DwmSetWindowAttributeDelay(hWnd,
				DWMWA_NCRENDERING_POLICY,
				&ncrp,
				sizeof(ncrp));

			// Nuke the theming
			SetWindowThemeDelay(hWnd, L"", L"");
			RedrawWindow(hWnd, NULL, NULL, RDW_FRAME);
		}
	}

	return;
}
