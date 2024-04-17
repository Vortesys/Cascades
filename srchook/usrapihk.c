/* * * * * * * *\
	USRAPIHK.C -
		Copyright © 2024 Brady McDermott, Vortesys
	DESCRIPTION -
		User32 UserApiHook function interface functions
		for NT Style.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "usrapihk.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/* Functions */
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
