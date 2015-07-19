#include "..\..\com\stksocket\stksocket.h"
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmdfrksrv.h"
#include "confaccess.h"
#include "netaccess.h"
#include "dataaccess.h"
#include "WorkerThread.h"
#include "..\..\com\msgproc\msgproc.h"


#define SERVICE_NAME (TEXT("CmdFreak Service"))

SERVICE_TABLE_ENTRY ServiceTable[] = {
	{ SERVICE_NAME, ServiceMain },
	{ NULL, NULL }
};

BOOL g_bRun = TRUE;
BOOL g_bService = TRUE;
SERVICE_STATUS_HANDLE g_hServiceStatus = NULL;
PROCESS_INFORMATION pi;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)lParam;

    DWORD lpdwProcessId = 0;
    GetWindowThreadProcessId(hWnd, &lpdwProcessId);
    if (pi->dwProcessId == lpdwProcessId) {
        PostMessage(hWnd, WM_CLOSE, 0, 0);
        return FALSE;
    }
    return TRUE;
}

void StartMongoose()
{
	STARTUPINFO si;
	TCHAR Buf[256];
	GetModuleFileName(NULL, Buf, 255);
	LPTSTR Addr = StrStr(Buf, _T("\\cmdfrksrv.exe"));
	lstrcpy(Addr, _T(""));

	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLine[512];
	wsprintf(CmdLine, _T("%s\\mongoose-3.7.exe"), Buf);
	CreateProcess(NULL, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
}

void StopMongoose()
{
	EnumWindows(EnumWindowsProc, (LPARAM)&pi);
	if (WaitForSingleObject(pi.hProcess, 5000) == WAIT_TIMEOUT) {
		TerminateProcess(pi.hProcess, 0);
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

DWORD WINAPI HandlerEx (DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
	// Initialize Variables for Service Control
	SERVICE_STATUS ss;
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwWin32ExitCode = NO_ERROR;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 1;
	ss.dwWaitHint = 1000;
	ss.dwControlsAccepted = 0;

	switch(dwControl) {
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:

		{
			StopMongoose();
			DataAccess* DatAc = DataAccess::GetInstance();
			DatAc->AddLogMsg(MessageProc::GetMsg(MessageProc::CMDFRK_SVCSTOP));
		}

		// Set STOP_PENDING status.
		ss.dwCurrentState = SERVICE_STOP_PENDING;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}

		// SERVICE SPECIFIC STOPPING CODE HERE.
		// ...
		// ...
		g_bService = FALSE;
		Sleep(2 * 1000);

		// Set STOPPED status.
		ss.dwCurrentState = SERVICE_STOPPED;
		ss.dwCheckPoint = 0;
		ss.dwWaitHint = 0;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}
		break;

	case SERVICE_CONTROL_PAUSE:

		// Set PAUSE_PENDING status.
		ss.dwCurrentState = SERVICE_PAUSE_PENDING;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}

		// APPLICATION SPECIFIC PAUSE_PENDING CODE HERE.
		// ...
		// ...
		g_bRun = FALSE;
		Sleep(2 * 1000);

		// Set PAUSE_PENDING status.
		ss.dwCurrentState = SERVICE_PAUSED;
		ss.dwCheckPoint = 0;
		ss.dwWaitHint = 0;
		ss.dwControlsAccepted =
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN |
			SERVICE_ACCEPT_STOP;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}
		break;

	case SERVICE_CONTROL_CONTINUE:

		// Set PAUSE_PENDING status.
		ss.dwCurrentState = SERVICE_START_PENDING;
		if (!SetServiceStatus (g_hServiceStatus, &ss)) {
			break;
		}

		// APPLICATION SPECIFIC START_PENDING CODE HERE.
		// ...
		// ...
		g_bRun = TRUE;
		Sleep(2 * 1000);

		// Set RUNNING status.
		ss.dwCurrentState = SERVICE_RUNNING;
		ss.dwCheckPoint = 0;
		ss.dwWaitHint = 0;
		ss.dwControlsAccepted =
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN |
			SERVICE_ACCEPT_STOP;
		if (!SetServiceStatus(g_hServiceStatus, &ss)) {
			break;
		}
		break;

	default:
		return ERROR_CALL_NOT_IMPLEMENTED;
	}

	return NO_ERROR;
}

VOID WINAPI ServiceMain(DWORD dwArgc, PTSTR* pszArgv)
{

	// Initialize Variables for Service Control
	SERVICE_STATUS ss;
	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwWin32ExitCode = NO_ERROR;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 1;
	ss.dwWaitHint = 1000;
	ss.dwControlsAccepted = 0;
	ss.dwCurrentState = SERVICE_START_PENDING;

	// Register Service Control Handler
	g_hServiceStatus = RegisterServiceCtrlHandlerEx(SERVICE_NAME, HandlerEx, NULL);
	if(0 == g_hServiceStatus) {
		return;
	}

	// Entering Starting Service.
	if (!SetServiceStatus(g_hServiceStatus, &ss)) {
		return;
	}

	// APPLICATION SPECIFIC INITIALIZATION CODE
	// ...
	// ...

	// Finish Initializing.
	ss.dwCurrentState = SERVICE_RUNNING;
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;
	ss.dwControlsAccepted = 
		SERVICE_ACCEPT_PAUSE_CONTINUE |
		SERVICE_ACCEPT_SHUTDOWN |
		SERVICE_ACCEPT_STOP;
	if (!SetServiceStatus(g_hServiceStatus, &ss)) {
		return;
	}

	// DataAccess instance
	DataAccess* DatAc = DataAccess::GetInstance();

	// Initialize data tables
	int RetCmdFrkTbl = DatAc->CreateCmdFreakTables();

	DatAc->AddLogMsg(MessageProc::GetMsg(MessageProc::CMDFRK_SVCSTART));
	StartMongoose();

	// Acquire host name and port number
	TCHAR Hostname[256];
	int PortNum;
	ConfAccess::GetInstance()->GetHostnameAndPortFromFile(Hostname, &PortNum);

	if (StkSocket_AddInfo(0, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_RECEIVER, Hostname, PortNum) != 0) {
		return;
	}
	if (StkSocket_Open(0) != 0) {
		return;
	}
	StartWorkerThread();
	while(g_bService) {
		if(!g_bRun) {
			Sleep(1000);
			continue;
		}
		NetAccess::GetInstance()->Dispatcher(0, RetCmdFrkTbl);
		Sleep(1);
	}
	StopWorkerThread();
	DatAc->StopAutoSave();
	if (StkSocket_Close(0, TRUE) != 0) {
		return;
	}
	if (StkSocket_DeleteInfo(0) != 0) {
		return;
	}
}

void main()
{

	BOOL bRet;

	bRet = StartServiceCtrlDispatcher(ServiceTable);

	printf_s("%s", MessageProc::GetMsgSjis(MessageProc::CMDFRK_CMDFRKSRV));
	printf_s("%s", MessageProc::GetMsgSjis(MessageProc::CMDFRK_CANNOT_START));

	return;
}

