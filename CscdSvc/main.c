/* * * * * * * *\
	MAIN.C -
		Copyright © 2024 Brady McDermott
	PROJECT - 
		Cascades Service
	DESCRIPTION -
		User32 UserApiHook testing program.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Includes */
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <strsafe.h>
#include "error.h"

/* Defines */
#define SVCNAME L"CscdSvc"
#define SVCDESC L"Cascades Theme Utility"

/* Global Variables */
SERVICE_STATUS          g_SvcStatus;
SERVICE_STATUS_HANDLE   g_SvcStatusHandle;
HANDLE                  g_hSvcStopEvent = NULL;

/* Functions */

/* * * *\
    wWinMain -
        Main entry point
\* * * */
INT WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    CscdSvcInstall();

	return 0;
}

/* * * *\
	CscdSvcInstall -
		Install the service in the SCM database
        Borrowed from Microsoft's Svc.cpp
\* * * */
VOID CscdSvcInstall()
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szUnquotedPath[MAX_PATH];

    if (!GetModuleFileName(NULL, szUnquotedPath, MAX_PATH))
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return;
    }

    // In case the path contains a space, it must be quoted so that
    // it is correctly interpreted. For example,
    // "d:\my share\myservice.exe" should be specified as
    // ""d:\my share\myservice.exe"".
    TCHAR szPath[MAX_PATH];
    StringCbPrintf(szPath, MAX_PATH, TEXT("\"%s\""), szUnquotedPath);

    // Get a handle to the SCM database. 
    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Create the service
    schService = CreateService(
        schSCManager,              // SCM database 
        SVCNAME,                   // name of service 
        SVCDESC,                   // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        szPath,                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 

    if (schService == NULL)
    {
        printf("CreateService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }
    else
    {
        printf("Service installed successfully\n");
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

/* * * *\
    CscdSvcMain -
        Install the service in the SCM database
        Borrowed from Microsoft's Svc.cpp
\* * * */
VOID WINAPI CscdSvcMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    // Register the handler function for the service
    g_SvcStatusHandle = RegisterServiceCtrlHandler(
        SVCNAME,
        CscdSvcCtrlHandler);

    // Return if the handler isn't installed
    if (!g_SvcStatusHandle)
    {
        CscdSvcReportEvent(TEXT("RegisterServiceCtrlHandler"));
        return;
    }

    // These SERVICE_STATUS members remain as set here
    g_SvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_SvcStatus.dwServiceSpecificExitCode = 0;

    // Report initial status to the SCM
    CscdReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    // Perform service-specific initialization and work.
    CscdSvcInit(dwArgc, lpszArgv);
}

/* * * *\
    CscdSvcInit -
        Service's initialization code
        Borrowed from Microsoft's Svc.cpp
    Parameters - 
        dwArgc - Number of arguments in the lpszArgv array
        lpszArgv - Array of strings. The first string is the name of
        the service and subsequent strings are passed by the process
        that called the StartService function to start the service.
\* * * */
VOID CscdSvcInit(DWORD dwArgc, LPTSTR* lpszArgv)
{
    // TO_DO: Declare and set any required variables.
    //   Be sure to periodically call CscdReportSvcStatus() with 
    //   SERVICE_START_PENDING. If initialization fails, call
    //   CscdReportSvcStatus with SERVICE_STOPPED.

    // Create an event. The control handler function, CscdSvcCtrlHandler,
    // signals this event when it receives the stop control code.
    g_hSvcStopEvent = CreateEvent(
        NULL,    // default security attributes
        TRUE,    // manual reset event
        FALSE,   // not signaled
        NULL);   // no name

    if (g_hSvcStopEvent == NULL)
    {
        CscdReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        return;
    }

    // Report running status when initialization is complete.
    CscdReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

    // TO_DO: Perform work until service stops.

    while (TRUE)
    {
        // Check whether to stop the service.
        WaitForSingleObject(g_hSvcStopEvent, INFINITE);

        CscdReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
        return;
    }
}

/* * * *\
    CscdReportSvcStatus -
        Sets the current service status and
        reports it to the SCM.
        Borrowed from Microsoft's Svc.cpp
    Parameters -
        dwCurrentState - The current state (see SERVICE_STATUS)
        dwWin32ExitCode - The system error code
        dwWaitHint - Estimated time for pending operation, 
        in milliseconds.
\* * * */
VOID CscdReportSvcStatus(DWORD dwCurrentState,
    DWORD dwWin32ExitCode,
    DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    // Fill in the SERVICE_STATUS structure.

    g_SvcStatus.dwCurrentState = dwCurrentState;
    g_SvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    g_SvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        g_SvcStatus.dwControlsAccepted = 0;
    else g_SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ((dwCurrentState == SERVICE_RUNNING) ||
        (dwCurrentState == SERVICE_STOPPED))
        g_SvcStatus.dwCheckPoint = 0;
    else g_SvcStatus.dwCheckPoint = dwCheckPoint++;

    // Report the status of the service to the SCM.
    SetServiceStatus(g_SvcStatusHandle, &g_SvcStatus);
}

/* * * *\
    CscdSvcCtrlHandler -
        Called by SCM whenever a control code is sent to the service
        using the ControlService function.
        Borrowed from Microsoft's Svc.cpp
    Parameters -
        dwCtrl - control code
\* * * */
VOID WINAPI CscdSvcCtrlHandler(DWORD dwCtrl)
{
    // Handle the requested control code. 
    switch (dwCtrl)
    {
    case SERVICE_CONTROL_STOP:
        CscdReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

        // Signal the service to stop.
        SetEvent(g_hSvcStopEvent);
        CscdReportSvcStatus(g_SvcStatus.dwCurrentState, NO_ERROR, 0);

        return;

    case SERVICE_CONTROL_INTERROGATE:
        break;

    default:
        break;
    }

}

/* * * *\
    CscdSvcReportEvent -
        Logs messages to the event log.
        The service must have an entry in the
        Application event log.
        Borrowed from Microsoft's Svc.cpp
    Parameters -
        szFunction - name of function that failed
\* * * */
VOID CscdSvcReportEvent(LPTSTR szFunction)
{
    HANDLE hEventSource;
    LPCTSTR lpszStrings[2];
    TCHAR Buffer[80];

    hEventSource = RegisterEventSource(NULL, SVCNAME);

    if (NULL != hEventSource)
    {
        StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

        lpszStrings[0] = SVCNAME;
        lpszStrings[1] = Buffer;

        ReportEvent(hEventSource,        // event log handle
            EVENTLOG_ERROR_TYPE, // event type
            0,                   // event category
            SVC_ERROR,           // event identifier
            NULL,                // no security identifier
            2,                   // size of lpszStrings array
            0,                   // no binary data
            lpszStrings,         // array of strings
            NULL);               // no binary data

        DeregisterEventSource(hEventSource);
    }
}
