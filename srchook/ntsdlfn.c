/* * * * * * * *\
	NTSDLFN.C -
		Copyright © 2024 Brady McDermott, Vortesys
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

/* Definitions */
#define STATUS_UNSUCCESSFUL 0xC0000001

/* Functions */

/* * * *\
	DwmGetWindowAttributeDelay -
		Gets the value of Desktop Window Manager
		(DWM) non-client rendering attributes
		for a window.
	RETURNS -
		NTSTATUS result.
\* * * */
HRESULT DwmGetWindowAttributeDelay(
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
		FARPROC fLib = GetProcAddress(hLib, "DwmGetWindowAttribute");

		hRet = (HRESULT)fLib(hwnd, dwAttribute, pvAttribute, cbAttribute);

		FreeLibrary(hLib);

		if (SUCCEEDED(hRet))
			return hRet;
	}

	return STATUS_UNSUCCESSFUL;
}

/* * * *\
	DwmSetWindowAttributeDelay -
		Sets the value of Desktop Window Manager
		(DWM) non-client rendering attributes
		for a window.
	RETURNS -
		NTSTATUS result.
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

		hRet = (HRESULT)fLib(hwnd, dwAttribute, pvAttribute, cbAttribute);

		FreeLibrary(hLib);

		if (SUCCEEDED(hRet))
			return hRet;
	}

	return STATUS_UNSUCCESSFUL;
}

/* * * *\
	DwmSetWindowAttributeDelay -
		Sets the value of Desktop Window Manager
		(DWM) non-client rendering attributes
		for a window.
	RETURNS -
		NTSTATUS result.
\* * * */
HRESULT SetWindowThemeDelay(
	_In_ HWND		hwnd,
	_In_ LPCWSTR	pszSubAppName,
	_In_ LPCWSTR	pszSubIdList
)
{
	HMODULE hLib = LoadLibrary(L"UxTheme.dll");
	HRESULT hRet = 0;

	if (hLib)
	{
		FARPROC fLib = GetProcAddress(hLib, "SetWindowTheme");

		hRet = (HRESULT)fLib(hwnd, pszSubAppName, pszSubIdList);

		FreeLibrary(hLib);

		if (SUCCEEDED(hRet))
			return hRet;
	}

	return STATUS_UNSUCCESSFUL;
}
