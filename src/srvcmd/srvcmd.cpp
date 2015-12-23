#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>
#include <wtsapi32.h>
#include "..\..\..\YaizuComLib\src\commonfunc\msgproc.h"

#define CMDFRK_SVCFAILEDTOSTART 3142
#define CMDFRK_SVCFAILEDTOSTOP 3143
#define CMDFRK_SVCALREADYSTART 3144
#define CMDFRK_SVCALREADYSTOP 3145
#define CMDFRK_SVCSTART 3146
#define CMDFRK_SVCSTOP 3147

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

	//3142 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCFAILEDTOSTART, _T("CmdFreakサービスの開始に失敗しました。"));
	MessageProc::AddEng(CMDFRK_SVCFAILEDTOSTART, _T("CmdFreak service initiation failed."));

	//3143 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCFAILEDTOSTOP, _T("CmdFreakサービスの停止に失敗しました。"));
	MessageProc::AddEng(CMDFRK_SVCFAILEDTOSTOP, _T("CmdFreak service termination failed."));

	//3144 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCALREADYSTART, _T("CmdFreakサービスはすでに開始しています。"));
	MessageProc::AddEng(CMDFRK_SVCALREADYSTART, _T("CmdFreak service has already been started."));

	//3145 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCALREADYSTOP, _T("CmdFreakサービスはすでに停止しています。"));
	MessageProc::AddEng(CMDFRK_SVCALREADYSTOP, _T("CmdFreak service has already been stopped."));

	//3146 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTART, _T("CmdFreakサービスを開始しました。"));
	MessageProc::AddEng(CMDFRK_SVCSTART, _T("CmdFreak service has started."));

	//3147 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTOP, _T("CmdFreakサービスを停止しました。"));
	MessageProc::AddEng(CMDFRK_SVCSTOP, _T("CmdFreak service has stopped."));


	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForNet[512];

	if (strstr(Param, "start") != 0) {
		if (IsProcessStarting() == TRUE) {
			MessageBox(NULL, MessageProc::GetMsg(CMDFRK_SVCALREADYSTART), MessageProc::GetMsg(CMDFRK_SVCALREADYSTART), MB_OK);
			return 0;
		}
		wsprintf(CmdLineForNet, _T("\"%s\\net.exe\" start CMDFRKSRV"), SystemDir);
		CreateProcess(NULL, CmdLineForNet, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		if (IsProcessStarting() == TRUE) {
			MessageBox(NULL, MessageProc::GetMsg(CMDFRK_SVCSTART), MessageProc::GetMsg(CMDFRK_SVCSTART), MB_OK);
		} else {
			MessageBox(NULL, MessageProc::GetMsg(CMDFRK_SVCFAILEDTOSTART), MessageProc::GetMsg(CMDFRK_SVCFAILEDTOSTART), MB_OK);
		}
	}

	if (strstr(Param, "stop") != 0) {
		if (IsProcessStarting() == FALSE) {
			MessageBox(NULL, MessageProc::GetMsg(CMDFRK_SVCALREADYSTOP), MessageProc::GetMsg(CMDFRK_SVCALREADYSTOP), MB_OK);
			return 0;
		}
		wsprintf(CmdLineForNet, _T("\"%s\\net.exe\" stop CMDFRKSRV"), SystemDir);
		CreateProcess(NULL, CmdLineForNet, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		if (IsProcessStarting() == FALSE) {
			MessageBox(NULL, MessageProc::GetMsg(CMDFRK_SVCSTOP), MessageProc::GetMsg(CMDFRK_SVCSTOP), MB_OK);
		} else {
			MessageBox(NULL, MessageProc::GetMsg(CMDFRK_SVCFAILEDTOSTOP), MessageProc::GetMsg(CMDFRK_SVCFAILEDTOSTOP), MB_OK);
		}
	}

	return 0;
}
