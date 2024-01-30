/* * * * * * * *\
	NTSDLFN.C -
		Copyright © 2023 Brady McDermott, Vortesys
	DESCRIPTION -
		Delay loaded functions, mostly for only testing
		and disabling DWM on systems newer than Windows
		Vista.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
 \* * * * * * * */

 /* Headers */
#include "ntshook.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/* Functions */

/* * * *\
	DwmSetWindowAttribute -
		Sets the value of Desktop Window Manager
		(DWM) non-client rendering attributes
		for a window.
	RETURNS -
		True if successful, false if unsuccessful.
\* * * */
HRESULT DwmSetWindowAttributeDelay(
	HWND			hwnd,
	DWORD			dwAttribute,
	_In_ LPCVOID	pvAttribute,
	DWORD			cbAttribute
)
{
	HMODULE hLib = LoadLibrary(L"Dwmapi.dll");
	HRESULT hRet = 0;

	if (hLib)
	{
		FARPROC fLib = GetProcAddress(hLib, "DwmSetWindowAttribute");
		if (hRet = (HRESULT)fLib(hwnd, dwAttribute, pvAttribute, cbAttribute))
		{
			FreeLibrary(hLib);
			return TRUE;
		}
	}

	return hRet;
}