/* * * * * * * *\
	NTSHOOK.C -
		Copyright © 2023 Brady McDermott, Vortesys
	DESCRIPTION -
		Defines the entry point for the DLL application.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
 \* * * * * * * */

// Includes
#include "ntshook.h"
#include "resource.h"
#include <dwmapi.h>
#include <uxtheme.h>

// Global Variables
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
	NTStyleHookProc -
		NT Style Hook procedure
		Uses WH_CALLWNDPROC.
\* * * */
__declspec(dllexport) LRESULT APIENTRY NTStyleHookProc(
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
		case WM_CREATE:
			NTStyleDisableWindowTheme(pcwps->hwnd);
		case WM_DISPLAYCHANGE:
		case WM_SYNCPAINT:
		case WM_ACTIVATE:
		case WM_PAINT:
		case WM_MOVE:
			NTStyleDrawWindow(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
			break;

		case WM_NCACTIVATE:
		case WM_NCCALCSIZE:
		case WM_NCPAINT:
			NTStyleDrawWindow(pcwps->hwnd, pcwps->wParam, pcwps->lParam);
			return 0;

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
	NTStyleSetHook -
		NT Style Hook hook so
		that we can actually
		WOW these programs.
\* * * */
__declspec(dllexport) HHOOK APIENTRY NTStyleSetHook(_In_ INT idHook, _In_ HOOKPROC lpfn, _In_ HINSTANCE hmod, _In_ DWORD dwThreadId)
{
	return SetWindowsHookEx(idHook, lpfn, hmod, dwThreadId);
}

/* * * *\
	NTStyleLockWindowUpdates -
		Prevents a window from
		updating for a small
		period of time to prevent
		flickering issues.
\* * * */
VOID NTStyleLockWindowUpdates(_In_ HWND hWnd)
{
	if (++g_iLockUpdateCount == 1) {
		SetWindowLong(hWnd, GWL_STYLE,
			GetWindowLong(hWnd, GWL_STYLE) & ~WS_VISIBLE);
	}

	return;
}

/* * * *\
	NTStyleUnlockWindowUpdates -
		Prevents a window from
		updating for a small
		period of time to prevent
		flickering issues.
\* * * */
VOID NTStyleUnlockWindowUpdates(_In_ HWND hWnd)
{
	if (--g_iLockUpdateCount <= 0) {
		SetWindowLong(hWnd, GWL_STYLE,
			GetWindowLong(hWnd, GWL_STYLE) | WS_VISIBLE);
		g_iLockUpdateCount = 0;
	}

	return;
}

/* * * *\
	NTStyleDisableWindowTheme -
		de-themify that window
\* * * */
__declspec(dllexport) VOID APIENTRY NTStyleDisableWindowTheme(_In_ HWND hWnd)
{
	enum DWMNCRENDERINGPOLICY ncrp = DWMNCRP_DISABLED;

	WCHAR pszClassName[256] = L"";

	// DO NOT DO ANY OF THIS TO EXPLORER!!!
	// EXPLORER NEEDS DWM!!!
	GetClassName(hWnd, (LPWSTR)pszClassName, ARRAYSIZE(pszClassName));

	if(pszClassName != (LPWSTR)L"ImmersiveLauncher")
		// Nuke DWM
		DwmSetWindowAttributeDelay(hWnd,
			DWMWA_NCRENDERING_POLICY,
			&ncrp,
			sizeof(ncrp));

	// Nuke the theming
	SetWindowTheme(hWnd, L"", L"");
	RedrawWindow(hWnd, NULL, NULL, RDW_FRAME);

	return;
}