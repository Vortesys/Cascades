/* * * * * * * *\
	SVC.C -
		Copyright © 2024 Brady McDermott
	DESCRIPTION -
		Cascades' Service component, adapted from
		https://learn.microsoft.com/en-us/windows/win32/services/the-complete-service-sample
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Includes */
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "svc.h"
#include "hook.h"
#include "..\CscdCom\error.h"

/* Defines */
#define SVCNAME TEXT("CscdSvc")
#define SVCDESC TEXT("Cascades' Theme Service")

/* Variables */
SERVICE_STATUS          gSvcStatus;
SERVICE_STATUS_HANDLE   gSvcStatusHandle;
HANDLE                  ghSvcStopEvent = NULL;

/* Functions */

//
// Purpose: 
//   Entry point for the process
//
// Parameters:
//   None
// 
// Return value:
//   None, defaults to 0 (zero)
//
int __cdecl _tmain(int argc, TCHAR* argv[])
{
	// If command-line parameter is "install", install the service. 
	// Otherwise, the service is probably being started by the SCM.
	if (lstrcmpi(argv[1], TEXT("install")) == 0)
	{
		SvcInstall();
		return 0;
	}

	// TO_DO: Add any additional services for the process to this table.
	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{ SVCNAME, (LPSERVICE_MAIN_FUNCTION)SvcMain },
		{ NULL, NULL }
	};

	// This call returns when the service has stopped. 
	// The process should simply terminate when the call returns.
	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		SvcReportEvent(TEXT("StartServiceCtrlDispatcher"));
	}
}

//
// Purpose: 
//   Installs a service in the SCM database
//   and registers the DLL in the event log
//
// Parameters:
//   None
// 
// Return value:
//   None
//
VOID SvcInstall()
{
	SC_HANDLE schSCManager;
	SC_HANDLE schService;
	TCHAR szUnquotedPath[MAX_PATH];
	SERVICE_DESCRIPTION sd;
	LPTSTR szDesc = TEXT("Hosts and maintains the Cascades Theme Utility.");

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
		NULL,                       // local computer
		SERVICES_ACTIVE_DATABASE,   // ServicesActive database 
		SC_MANAGER_ALL_ACCESS);     // full access rights 

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
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type 
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
	else printf("Service installed successfully\n");

	// Change the service description.
	sd.lpDescription = szDesc;

	if (!ChangeServiceConfig2(
		schService,                 // handle to service
		SERVICE_CONFIG_DESCRIPTION, // change: description
		&sd))                       // new description
	{
		printf("ChangeServiceConfig2 failed\n");
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	// Add ourselves to the registry for the event log
	HKEY hKeyEventLog = NULL;
	DWORD dwKeyDisposition = 0;
	DWORD dwEventLogMask = 0x7;

	if (RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		TEXT("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\CscdSvc"),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hKeyEventLog,
		&dwKeyDisposition
	) != ERROR_SUCCESS)
	{
		printf("RegCreateKeyEx failed (%d)\n", GetLastError());
		return;
	}

	// Create the subkeys
	if (hKeyEventLog != NULL)
	{
		if (dwKeyDisposition == REG_OPENED_EXISTING_KEY)
		{
			printf("Event Log key already exists\n");
			RegCloseKey(hKeyEventLog);
			return;
		}

		if (RegSetValueEx(hKeyEventLog, TEXT("EventMessageFile"), 0, REG_SZ, (const BYTE *)szPath, sizeof(szPath)) != ERROR_SUCCESS)
		{
			printf("RegSetValueEx (STRING) failed (%d)\n", GetLastError());
			RegCloseKey(hKeyEventLog);
			return;
		}

		if (RegSetValueEx(hKeyEventLog, TEXT("TypesSupported"), 0, REG_DWORD, (const BYTE*)&dwEventLogMask, sizeof(DWORD)) != ERROR_SUCCESS)
		{
			printf("RegSetValueEx (DWORD) failed (%d)\n", GetLastError());
			RegCloseKey(hKeyEventLog);
			return;
		}
	}

	// Verify we've registered ourselves in the event log
	HANDLE hEventLog = NULL;

	hEventLog = RegisterEventSource(NULL, SVCNAME);

	if (hEventLog == NULL)
	{
		printf("RegisterEventSource failed with 0x%x.\n", GetLastError());
	}

	if (hEventLog)
		DeregisterEventSource(hEventLog);
}

//
// Purpose: 
//   Entry point for the service
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None.
//
VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	// Register the handler function for the service
	gSvcStatusHandle = RegisterServiceCtrlHandler(
		SVCNAME,
		SvcCtrlHandler);

	if (!gSvcStatusHandle)
	{
		SvcReportEvent(TEXT("RegisterServiceCtrlHandler"));
		return;
	}

	// These SERVICE_STATUS members remain as set here
	gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	gSvcStatus.dwServiceSpecificExitCode = 0;

	// Report initial status to the SCM
	ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	// Perform service-specific initialization and work.
	SvcInit(dwArgc, lpszArgv);
}

//
// Purpose: 
//   The service code
//
// Parameters:
//   dwArgc - Number of arguments in the lpszArgv array
//   lpszArgv - Array of strings. The first string is the name of
//     the service and subsequent strings are passed by the process
//     that called the StartService function to start the service.
// 
// Return value:
//   None
//
VOID SvcInit(DWORD dwArgc, LPTSTR* lpszArgv)
{
	// TO_DO: Declare and set any required variables.
	//   Be sure to periodically call ReportSvcStatus() with 
	//   SERVICE_START_PENDING. If initialization fails, call
	//   ReportSvcStatus with SERVICE_STOPPED.

	// Create an event. The control handler function, SvcCtrlHandler,
	// signals this event when it receives the stop control code.
	ghSvcStopEvent = CreateEvent(
		NULL,    // default security attributes
		TRUE,    // manual reset event
		FALSE,   // not signaled
		NULL);   // no name

	if (ghSvcStopEvent == NULL)
	{
		ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
		return;
	}

	// Report running status when initialization is complete.
	ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

	// TO_DO: Perform work until service stops.

	// Install our user hook, if FALSE then fail.
	if (!InstallUserHook())
	{
		SvcMessageEvent(TEXT("RegisterUserApiHook"));

		ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
		return;
	}

	SvcMessageEvent(TEXT("InstallUserHook"));

	while (TRUE)
	{
		// Check whether to stop the service.
		WaitForSingleObject(ghSvcStopEvent, INFINITE);

		ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
		return;
	}

	// Kill the user hook
	if (!RemoveUserHook())
	{
		SvcMessageEvent(TEXT("UnregisterUserApiHook"));

		ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
		return;
	}
}

//
// Purpose: 
//   Sets the current service status and reports it to the SCM.
//
// Parameters:
//   dwCurrentState - The current state (see SERVICE_STATUS)
//   dwWin32ExitCode - The system error code
//   dwWaitHint - Estimated time for pending operation, 
//     in milliseconds
//
// Return value:
//   None
//
VOID ReportSvcStatus(DWORD dwCurrentState,
	DWORD dwWin32ExitCode,
	DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;

	// Fill in the SERVICE_STATUS structure.=
	gSvcStatus.dwCurrentState = dwCurrentState;
	gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
	gSvcStatus.dwWaitHint = dwWaitHint;

	if (dwCurrentState == SERVICE_START_PENDING)
		gSvcStatus.dwControlsAccepted = 0;
	else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	if ((dwCurrentState == SERVICE_RUNNING) ||
		(dwCurrentState == SERVICE_STOPPED))
		gSvcStatus.dwCheckPoint = 0;
	else gSvcStatus.dwCheckPoint = dwCheckPoint++;

	// Report the status of the service to the SCM.
	SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

//
// Purpose: 
//   Called by SCM whenever a control code is sent to the service
//   using the ControlService function.
//
// Parameters:
//   dwCtrl - control code
//
// Return value:
//   None
//
VOID WINAPI SvcCtrlHandler(DWORD dwCtrl)
{
	// Handle the requested control code.
	switch (dwCtrl)
	{
	case SERVICE_CONTROL_STOP:
		ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);

		// Signal the service to stop.
		SetEvent(ghSvcStopEvent);
		ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);

		return;

	case SERVICE_CONTROL_INTERROGATE:
		break;

	default:
		break;
	}

}

//
// Purpose: 
//   Logs messages to the event log
//
// Parameters:
//   szFunction - name of function that failed
//
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcReportEvent(LPTSTR szFunction)
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


//
// Purpose: 
//   Show a message window for debugging
//
// Parameters:
//   szFunction - name of function that failed
//
// Return value:
//   None
//
// Remarks:
//   The service must have an entry in the Application event log.
//
VOID SvcMessageEvent(LPTSTR szFunction)
{
	TCHAR Buffer[80];

	StringCchPrintf(Buffer, 80, TEXT("%s failed with %d"), szFunction, GetLastError());

	MessageBox(NULL, Buffer, SVCNAME, MB_SERVICE_NOTIFICATION | MB_OK);

	return;
}
