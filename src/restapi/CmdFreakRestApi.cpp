#include "..\..\..\YaizuComLib\src\stkpl\StkPl.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkObject.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkProperties.h"
#include "..\..\..\YaizuComLib\src\stkthread\stkthread.h"
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebApp.h"
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"
#include "dataaccess.h"
#include "MyMsgProc.h"
#include "ApiGetSystem.h"
#include "ApiGetLanguage.h"
#include "ApiLogging.h"
#include "ApiOdbcInfo.h"
#include "ApiGetTableInfo.h"
#include "ApiGetRecord.h"
#include "ApiFilterInfo.h"
#include "ApiGetUser.h"

void CmdFreakRestApi(wchar_t* IpAddr, int Port, int SendBufSize)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 7, IpAddr, Port);
	Soc->SetSendBufSize(SendBufSize);

	ApiGetSystem* ApiGetSystemObj = new ApiGetSystem();
	int Add1 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/system/", (StkWebAppExec*)ApiGetSystemObj);
	ApiGetSystemObj->SetNumOfThreads(7);

	ApiGetLanguage* ApiGetLanguageObj = new ApiGetLanguage();
	int Add2 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/language/", (StkWebAppExec*)ApiGetLanguageObj);

	ApiLogging* ApiLoggingObj = new ApiLogging();
	int Add3 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/logs/", (StkWebAppExec*)ApiLoggingObj);

	ApiOdbcInfo* ApiGetOdbcInfoObj = new ApiOdbcInfo();
	int Add4 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/odbcinfo/$", (StkWebAppExec*)ApiGetOdbcInfoObj);

	ApiOdbcInfo* ApiPostOdbcInfoObj = new ApiOdbcInfo();
	int Add5 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/odbcinfo/", (StkWebAppExec*)ApiPostOdbcInfoObj);

	ApiGetTableInfo* ApiGetTableInfoObj = new ApiGetTableInfo();
	int Add6 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/", (StkWebAppExec*)ApiGetTableInfoObj);

	ApiGetTableInfo* ApiGetTableInfoByNameObj = new ApiGetTableInfo();
	int Add7 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/$", (StkWebAppExec*)ApiGetTableInfoByNameObj);

	ApiGetRecord* ApiGetRecordObj = new ApiGetRecord();
	int Add8 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/records/$", (StkWebAppExec*)ApiGetRecordObj);

	ApiFilterInfo* ApiGetFilterInfoObj = new ApiFilterInfo();
	int Add9 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filterinfo/", (StkWebAppExec*)ApiGetFilterInfoObj);

	ApiFilterInfo* ApiPostFilterInfoObj = new ApiFilterInfo();
	int Add10 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/filterinfo/", (StkWebAppExec*)ApiPostFilterInfoObj);

	ApiGetUser* ApiGetUserObj = new ApiGetUser();
	int Add11 = Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/user/", (StkWebAppExec*)ApiGetUserObj);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/system/");
	int Del2 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/language/");
	int Del3 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/logs/");
	int Del4 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/odbcinfo/$");
	int Del5 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/odbcinfo/");
	int Del6 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/");
	int Del7 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/$");
	int Del8 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/records/$");
	int Del9 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filterinfo/");
	int Del10 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/filterinfo/");
	int Del11 = Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/user/");

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	// Initialize messages
	MyMsgProc::AddMsg();

	// Display product name
	StkPlPrintf("%s\r\n", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_CMDFRKSRV));

	// Acquire current path
	wchar_t Buf[FILENAME_MAX];
	wchar_t BufWoFileName[FILENAME_MAX];

	StkPlGetFullPathFromFileName(L"\\sample.exe", Buf);
	if (StkPlGetFullPathWithoutFileName(Buf, BufWoFileName) == 0) {
		ChangeCurrentDirectory(BufWoFileName);
	} else {
		StkPlPrintf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_EXEC_NOT_FOUND));
		return -1;
	}
	/*
	GetModuleFileName(NULL, Buf, 255);
	wchar_t* Addr = (wchar_t*)StkPlWcsStr(Buf, L"\\sample.exe");
	if (Addr != NULL) {
		*Addr = L'\0';
		SetCurrentDirectory(Buf);
		StkPlPrintf("Current Directory = %S\r\n", Buf);
	} else {
		StkPlPrintf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_EXEC_NOT_FOUND));
		return -1;
	}
	*/

	wchar_t IpAddr[256];
	int Port;
	int SendBufSize = 20000000;

	// Load properties
	StkProperties *Prop = new StkProperties();
	if (Prop->GetProperties(L"sample.conf") == 0) {
		char IpAddrTmp[256];
		if (Prop->GetPropertyStr("servicehost", IpAddrTmp) != 0) {
			StkPlPrintf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_DAT_SERVICEHOST_NOT_FOUND));
			return -1;
		}
		StkPlPrintf("servicehost property = %s\r\n", IpAddrTmp);
		StkPlConvUtf8ToWideChar(IpAddr, 256, IpAddrTmp);

		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			StkPlPrintf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_DAT_SERVICEPORT_NOT_FOUND));
			return -1;
		}
		StkPlPrintf("serviceport property = %d\r\n", Port);

		if (Prop->GetPropertyInt("sendbufsize", &SendBufSize) != 0) {
			// nothing to do
		}
		StkPlPrintf("send buffer size = %d\r\n", SendBufSize);
	} else {
		StkPlPrintf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_CONF_NOT_FOUND));
		return -1;
	}
	StkPlPrintf("\r\n");

	// DataAccess instance
	DataAccess* DatAc = DataAccess::GetInstance();

	// Initialize data tables and start AutoSave
	if (DatAc->CreateCmdFreakTables() != 0) {
		StkPlPrintf("%s", MyMsgProc::GetMsgSjis(MyMsgProc::CMDFRK_DAT_NOT_FOUND));
		return -1;
	}

	DatAc->AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTART), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTART));

	// Exec rest api
	CmdFreakRestApi(IpAddr, Port, SendBufSize);

	DatAc->AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTOP), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTOP));

	// Stop AutoSave
	DatAc->StopAutoSave();

	return 0;
}
