/* * * * * * * *\
	SVC.H -
		Copyright © 2024 Brady McDermott
	DESCRIPTION -
		Cascades' Service component header
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "advapi32.lib")

/* Function Prototypes */
VOID SvcInstall(void);
VOID WINAPI SvcCtrlHandler(DWORD);
VOID WINAPI SvcMain(DWORD, LPTSTR*);

VOID ReportSvcStatus(DWORD, DWORD, DWORD);
VOID SvcInit(DWORD, LPTSTR*);
VOID SvcReportEvent(LPTSTR);
VOID SvcMessageEvent(LPTSTR);
