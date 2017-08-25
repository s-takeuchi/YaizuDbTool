#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <stdio.h>
#include <Psapi.h>
#include "..\..\..\YaizuComLib\src\commonfunc\StkObject.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkProperties.h"
#include "..\..\..\YaizuComLib\src\stkthread\stkthread.h"
#include "..\..\..\YaizuComLib\src\stksocket\stksocket.h"
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebApp.h"
#include "dataaccess.h"
#include "MyMsgProc.h"
#include "ApiGetSystem.h"
#include "ApiGetLanguage.h"
#include "ApiLogging.h"
#include "ApiOdbcInfos.h"

void CmdFreakRestApi(TCHAR* IpAddr, int Port)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 7, IpAddr, Port);

	ApiGetSystem* ApiGetSystemObj = new ApiGetSystem();
	int Add1 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/system/"), (StkWebAppExec*)ApiGetSystemObj);
	ApiGetSystemObj->SetNumOfThreads(7);
	ApiGetLanguage* ApiGetLanguageObj = new ApiGetLanguage();
	int Add2 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/language/"), (StkWebAppExec*)ApiGetLanguageObj);
	ApiLogging* ApiLoggingObj = new ApiLogging();
	int Add3 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/logs/"), (StkWebAppExec*)ApiLoggingObj);
	ApiOdbcInfos* ApiOdbcInfoObj = new ApiOdbcInfos();
	int Add4 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/odbcinfos/$"), (StkWebAppExec*)ApiOdbcInfoObj);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/system/"));
	int Del2 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/language/"));
	int Del3 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/logs/"));
	int Del4 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/odbcinfos/"));

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	// Initialize messages
	MyMsgProc::AddMsg();

	// Display product name
	printf_s("%s\r\n", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_CMDFRKSRV));

	// Acquire current path
	TCHAR Buf[256];
	GetModuleFileName(NULL, Buf, 255);
	LPTSTR Addr = StrStr(Buf, _T("\\stkwebapp.exe"));
	lstrcpy(Addr, _T(""));
	SetCurrentDirectory(Buf);
	printf("Current Directory = %S\r\n", Buf);

	TCHAR IpAddr[256];
	int Port;

	// Load properties
	StkProperties *Prop = new StkProperties();
	if (Prop->GetProperties(_T("stkwebapp.conf")) == 0) {
		char IpAddrTmp[256];
		if (Prop->GetPropertyStr("servicehost", IpAddrTmp) != 0) {
			printf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_DAT_SERVICEHOST_NOT_FOUND));
			return -1;
		}
		printf("servicehost property = %s\r\n", IpAddrTmp);
		wsprintf(IpAddr, _T("%S"), IpAddrTmp);
		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			printf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_DAT_SERVICEPORT_NOT_FOUND));
			return -1;
		}
		printf("serviceport property = %d\r\n", Port);
	} else {
		printf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_CONF_NOT_FOUND));
		return -1;
	}

	// DataAccess instance
	DataAccess* DatAc = DataAccess::GetInstance();

	// Initialize data tables and start AutoSave
	if (DatAc->CreateCmdFreakTables() != 0) {
		printf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_DAT_NOT_FOUND));
		return -1;
	}

	DatAc->AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTART), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTART));

	// Exec rest api
	CmdFreakRestApi(IpAddr, Port);

	DatAc->AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTOP), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTOP));

	// Stop AutoSave
	DatAc->StopAutoSave();

	return 0;
}
