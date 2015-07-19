#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	TCHAR Buf[256];
	GetModuleFileName(NULL, Buf, 255);
	LPTSTR Addr = StrStr(Buf, _T("\\prog_del.exe"));
	lstrcpy(Addr, _T(""));

	TCHAR SystemDir[MAX_PATH];
	GetSystemDirectory(SystemDir, MAX_PATH);

	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLine[512];
	wsprintf(CmdLine, _T("\"%s\\netsh.exe\" firewall delete allowedprogram \"%s\\cmdfrksrv.exe\""), SystemDir, Buf);
	CreateProcess(NULL, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	wsprintf(CmdLine, _T("\"%s\\netsh.exe\" firewall delete allowedprogram \"%s\\mongoose-3.7.exe\""), SystemDir, Buf);
	CreateProcess(NULL, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForNet[512];
	wsprintf(CmdLineForNet, _T("\"%s\\net.exe\" stop CMDFRKSRV"), SystemDir);
	CreateProcess(NULL, CmdLineForNet, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForService[512];
	wsprintf(CmdLineForService, _T("\"%s\\sc.exe\" delete CMDFRKSRV"), SystemDir);
	CreateProcess(NULL, CmdLineForService, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	// Delete cmdfrksrv.exe
	TCHAR PathToDelFile[MAX_PATH];
	wsprintf(PathToDelFile, _T("%s\\cmdfrksrv.exe"), Buf);
	DeleteFile(PathToDelFile);
	wsprintf(PathToDelFile, _T("%s\\mongoose-3.7.exe"), Buf);
	DeleteFile(PathToDelFile);

	return 0;
}
