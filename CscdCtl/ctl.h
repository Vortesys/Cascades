/* * * * * * * *\
    CTL.H -
        Copyright © 2024 Brady McDermott
    DESCRIPTION -
        Cascades' Service Control component header
    LICENSE INFORMATION -
        MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "advapi32.lib")

/* Function Prototypes */
VOID WINAPI DisplayUsage(void);

VOID WINAPI DoStartSvc(void);
VOID WINAPI DoUpdateSvcDacl(void);
VOID WINAPI DoStopSvc(void);

BOOL WINAPI StopDependentServices(void);
