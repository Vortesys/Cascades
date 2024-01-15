/* * * * * * * *\
	NTSTYLE.C -
		Copyright © 2023 Brady McDermott, Vortesys
	DESCRIPTION -
		Defines the entry point for the DLL application.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
 \* * * * * * * */

// Includes
#include "ntshook.h"

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
\* * * */
__declspec(dllexport) LRESULT NTStyleHookProc(
	_In_ int nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (nCode < 0)  // send it on down the line
		return CallNextHookEx(NULL, nCode, wParam, lParam);

	//MessageBox(HWND_DESKTOP, L"NC repaint", L"NT Style Hook", MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
