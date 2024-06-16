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
#include "cscdhook.h"
#include "..\common\usrapihk.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/* Functions */

/* * * *\
	RegisterUserApiHookDelay -
		Registers a DLL and its initialization function
		to User32 for theming controls.
	RETURNS -
		TRUE if successful.
\* * * */
BOOL WINAPI RegisterUserApiHookDelay(PUSERAPIHOOKINFO ApiHookInfo)
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
	UnregisterUserApiHookDelay -
		Unregisters a hook from User32.
	RETURNS -
		TRUE if successful.
\* * * */
BOOL WINAPI UnregisterUserApiHookDelay(VOID)
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
