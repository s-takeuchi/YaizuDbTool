#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>
#include <wtsapi32.h>
#include "..\..\..\YaizuComLib\src\msgproc\msgproc.h"

BOOL DebugOption = FALSE;

BOOL IsProcessStarting()
{
	PWTS_PROCESS_INFO ProcessInfo;
	DWORD ProcCount = 0;
	WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, &ProcessInfo, &ProcCount);
	for (int Loop = 0; Loop < (int)ProcCount; Loop++) {
		if (lstrcmp(ProcessInfo[Loop].pProcessName, _T("cmdfrksrv.exe")) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	TCHAR SystemDir[MAX_PATH];
	GetSystemDirectory(SystemDir, MAX_PATH);

	char Param[256] = "";
	sscanf_s(lpCmdLine, "%s", Param);


	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForNet[512];

	if (strstr(Param, "start") != 0) {
		if (IsProcessStarting() == TRUE) {
			MessageBox(NULL, MessageProc::GetMsg(MessageProc::CMDFRK_SVCALREADYSTART), MessageProc::GetMsg(MessageProc::CMDFRK_SVCALREADYSTART), MB_OK);
			return 0;
		}
		wsprintf(CmdLineForNet, _T("\"%s\\net.exe\" start CMDFRKSRV"), SystemDir);
		CreateProcess(NULL, CmdLineForNet, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		if (IsProcessStarting() == TRUE) {
			MessageBox(NULL, MessageProc::GetMsg(MessageProc::CMDFRK_SVCSTART), MessageProc::GetMsg(MessageProc::CMDFRK_SVCSTART), MB_OK);
		} else {
			MessageBox(NULL, MessageProc::GetMsg(MessageProc::CMDFRK_SVCFAILEDTOSTART), MessageProc::GetMsg(MessageProc::CMDFRK_SVCFAILEDTOSTART), MB_OK);
		}
	}

	if (strstr(Param, "stop") != 0) {
		if (IsProcessStarting() == FALSE) {
			MessageBox(NULL, MessageProc::GetMsg(MessageProc::CMDFRK_SVCALREADYSTOP), MessageProc::GetMsg(MessageProc::CMDFRK_SVCALREADYSTOP), MB_OK);
			return 0;
		}
		wsprintf(CmdLineForNet, _T("\"%s\\net.exe\" stop CMDFRKSRV"), SystemDir);
		CreateProcess(NULL, CmdLineForNet, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		if (IsProcessStarting() == FALSE) {
			MessageBox(NULL, MessageProc::GetMsg(MessageProc::CMDFRK_SVCSTOP), MessageProc::GetMsg(MessageProc::CMDFRK_SVCSTOP), MB_OK);
		} else {
			MessageBox(NULL, MessageProc::GetMsg(MessageProc::CMDFRK_SVCFAILEDTOSTOP), MessageProc::GetMsg(MessageProc::CMDFRK_SVCFAILEDTOSTOP), MB_OK);
		}
	}

	return 0;
}
