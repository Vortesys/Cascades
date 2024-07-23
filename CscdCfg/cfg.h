/* * * * * * * *\
    CFG.H -
        Copyright © 2024 Brady McDermott
    DESCRIPTION -
        Cascades' Service Configuration component header
    LICENSE INFORMATION -
        MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once
#pragma comment(lib, "advapi32.lib")

/* Function Prototypes */
VOID WINAPI DisplayUsage(void);

VOID WINAPI DoQuerySvc(void);
VOID WINAPI DoUpdateSvcDesc(void);
VOID WINAPI DoDisableSvc(void);
VOID WINAPI DoEnableSvc(void);
VOID WINAPI DoDeleteSvc(void);
