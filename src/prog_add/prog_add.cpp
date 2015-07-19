#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>

BOOL DebugOption = FALSE;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	TCHAR Buf[256];
	GetModuleFileName(NULL, Buf, 255);
	LPTSTR Addr = StrStr(Buf, _T("\\bin\\prog_add.exe"));
	lstrcpy(Addr, _T(""));

	TCHAR SystemDir[MAX_PATH];
	GetSystemDirectory(SystemDir, MAX_PATH);

	// Output web server parameters
	HANDLE FileHndl;
	int WebPortNum = 0;
	int ServicePortNum = 0;
	sscanf_s(lpCmdLine, "%d %d", &WebPortNum, &ServicePortNum);

	if (WebPortNum > 1 && ServicePortNum > 1) {
		TCHAR MongooseConfPath[MAX_PATH];
		swprintf(MongooseConfPath, MAX_PATH, _T("%s\\bin\\mongoose.conf"), Buf);
		FileHndl = CreateFile(MongooseConfPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (FileHndl != INVALID_HANDLE_VALUE) {
			char MongooseStr[4096];
			DWORD NumOfByteWrite;
			sprintf_s(MongooseStr, 4096, "document_root %S\\htdocs\r\nlistening_ports %d\r\nenable_directory_listing no\r\ncgi_pattern **.exe$\r\ncgi_interpreter %S\\htdocs\\bbb.exe\r\nindex_files bbb.exe\r\n", Buf, WebPortNum, Buf);
			WriteFile(FileHndl, MongooseStr, strlen(MongooseStr), &NumOfByteWrite, NULL);
			CloseHandle(FileHndl);
		};

		TCHAR BbbConfPath[MAX_PATH];
		swprintf(BbbConfPath, MAX_PATH, _T("%s\\htdocs\\bbb.conf"), Buf);
		FileHndl = CreateFile(BbbConfPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (FileHndl != INVALID_HANDLE_VALUE) {
			char BbbStr[4096];
			DWORD NumOfByteWrite;
			sprintf_s(BbbStr, 4096, "accesshost=localhost\r\naccessport=%d\r\n", ServicePortNum);
			WriteFile(FileHndl, BbbStr, strlen(BbbStr), &NumOfByteWrite, NULL);
			CloseHandle(FileHndl);
		};

		TCHAR CmdFreakConfPath[MAX_PATH];
		swprintf(CmdFreakConfPath, MAX_PATH, _T("%s\\bin\\cmdfrksrv.conf"), Buf);
		FileHndl = CreateFile(CmdFreakConfPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (FileHndl != INVALID_HANDLE_VALUE) {
			char CmdFreakStr[4096];
			DWORD NumOfByteWrite;
			sprintf_s(CmdFreakStr, 4096, "servicehost=localhost\r\nserviceport=%d\r\n", ServicePortNum);
			WriteFile(FileHndl, CmdFreakStr, strlen(CmdFreakStr), &NumOfByteWrite, NULL);
			CloseHandle(FileHndl);
		};
	}


	// Firewall exception registration
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLine[512];
	wsprintf(CmdLine, _T("\"%s\\netsh.exe\" firewall add allowedprogram \"%s\\bin\\cmdfrksrv.exe\" CmdFreak ENABLE"), SystemDir, Buf);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}
	wsprintf(CmdLine, _T("\"%s\\netsh.exe\" firewall add allowedprogram \"%s\\bin\\mongoose-3.7.exe\" mongoose ENABLE"), SystemDir, Buf);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}

	// Service creates
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForService[512];
	wsprintf(CmdLineForService, _T("\"%s\\sc.exe\" create CMDFRKSRV binPath= \"%s\\bin\\cmdfrksrv.exe\" start= auto displayname= \"CmdFreak\""), SystemDir, Buf);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLineForService, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}

	// Service starts
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForNet[512];
	wsprintf(CmdLineForNet, _T("\"%s\\net.exe\" start CMDFRKSRV"), SystemDir);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLineForNet, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}

	// Grand modify permission to cmdfrksrv.conf
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForIcacls[512];
	wsprintf(CmdLineForIcacls, _T("\"%s\\icacls.exe\" \"%s\\bin\\cmdfrksrv.conf\" /grant Users:M"), SystemDir, Buf);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLineForIcacls, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}

	// Grand modify permission to bbb.conf
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	wsprintf(CmdLineForIcacls, _T("\"%s\\icacls.exe\" \"%s\\htdocs\\bbb.conf\" /grant Users:M"), SystemDir, Buf);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLineForIcacls, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}

	// Grand modify permission to mongoose.conf
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	wsprintf(CmdLineForIcacls, _T("\"%s\\icacls.exe\" \"%s\\bin\\mongoose.conf\" /grant Users:M"), SystemDir, Buf);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLineForIcacls, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}

	// Grand modify permission to cmdfrksrv.dat
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	TCHAR CmdLineForIcaclsDat[512];
	wsprintf(CmdLineForIcaclsDat, _T("\"%s\\icacls.exe\" \"%s\\bin\\cmdfrksrv.dat\" /grant Users:M"), SystemDir, Buf);
	if (!DebugOption) {
		CreateProcess(NULL, CmdLineForIcaclsDat, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
	}

	return 0;
}
