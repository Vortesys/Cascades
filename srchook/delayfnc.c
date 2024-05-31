/* * * * * * * *\
	DELAYFNC.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		Delay loaded functions, mostly for only testing
		and disabling DWM on systems newer than Windows
		Vista. Also contains the RegisterUserApiHook.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

 /* Headers */
#include "ntshook.h"
#include "..\common\usrapihk.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

/* * * *\
	RegisterUserApiHook -
		Registers a DLL and its initialization function
		to User32 for theming controls.
	RETURNS -
		TRUE if successful.
\* * * */
BOOL WINAPI RegisterUserApiHook(PUSERAPIHOOKINFO ApiHookInfo)
{
	HMODULE hLib = LoadLibrary(L"user32.dll");
	BOOL bRet = 0;

	if (hLib)
	{
		FARPROC fLib = GetProcAddress(hLib, "RegisterUserApiHook");

		bRet = (BOOL)fLib(ApiHookInfo);

		FreeLibrary(hLib);

		return bRet;
	}

	return FALSE;
}

/* * * *\
	UnregisterUserApiHook -
		Unregisters a DLL from User32.
	RETURNS -
		TRUE if successful.
\* * * */
BOOL WINAPI UnregisterUserApiHook(VOID)
{
	HMODULE hLib = LoadLibrary(L"user32.dll");
	BOOL bRet = 0;

	if (hLib)
	{
		FARPROC fLib = GetProcAddress(hLib, "UnregisterUserApiHook");

		bRet = (BOOL)fLib();

		FreeLibrary(hLib);

		return bRet;
	}

	return FALSE;
}
