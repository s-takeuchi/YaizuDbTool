﻿#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebApp.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "dataaccess.h"
#include "Global.h"
#include "MyMsgProc.h"
#include "ApiGetSystem.h"
#include "ApiOdbcInfo.h"
#include "ApiGetTableInfo.h"
#include "ApiGetRecord.h"
#include "ApiGetRecCount.h"
#include "ApiFilterInfo.h"

void CmdFreakRestApi(wchar_t* IpAddr, int Port)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 7, IpAddr, Port);

	ApiGetSystem* ApiGetSystemObj = new ApiGetSystem();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/system/", (StkWebAppExec*)ApiGetSystemObj);
	ApiGetSystemObj->SetNumOfThreads(7);

	ApiOdbcInfo* ApiGetOdbcInfoObj = new ApiOdbcInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/odbcinfo/$", (StkWebAppExec*)ApiGetOdbcInfoObj);

	ApiOdbcInfo* ApiPostOdbcInfoObj = new ApiOdbcInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/odbcinfo/", (StkWebAppExec*)ApiPostOdbcInfoObj);

	ApiGetTableInfo* ApiGetTableInfoObj = new ApiGetTableInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/", (StkWebAppExec*)ApiGetTableInfoObj);

	ApiGetTableInfo* ApiGetTableInfoByNameObj = new ApiGetTableInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/$", (StkWebAppExec*)ApiGetTableInfoByNameObj);

	ApiGetRecord* ApiGetRecordObj = new ApiGetRecord();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/records/$", (StkWebAppExec*)ApiGetRecordObj);

	ApiGetRecCount* ApiGetRecCountObj = new ApiGetRecCount();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/reccount/$", (StkWebAppExec*)ApiGetRecCountObj);

	ApiFilterInfo* ApiGetFilterInfoObj = new ApiFilterInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filterinfo/", (StkWebAppExec*)ApiGetFilterInfoObj);

	ApiFilterInfo* ApiPostFilterInfoObj = new ApiFilterInfo();
	Soc->AddReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/filterinfo/", (StkWebAppExec*)ApiPostFilterInfoObj);

	StkWebAppUm_RegisterApi(Soc);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	StkWebAppUm_UnregisterApi(Soc);

	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/system/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/language/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/odbcinfo/$");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/odbcinfo/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/tableinfo/$");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/records/$");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/reccount/$");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_GET, L"/api/filterinfo/");
	Soc->DeleteReqHandler(StkWebAppExec::STKWEBAPP_METHOD_POST, L"/api/filterinfo/");

	delete Soc;
}

int main(int Argc, char* Argv[])
{
	// Initialize messages
	MyMsgProc::AddMsg();
	StkWebAppUm_Init();

	// Logging starts
	char LogBuf[1024] = "";
	wchar_t LoggingPath[FILENAME_MAX] = L"";
#ifdef WIN32
	StkPlGetFullPathFromFileName(L"cmdfreak.log", LoggingPath);
#else
	StkPlWcsCpy(LoggingPath, FILENAME_MAX, L"/var/log/cmdfreak.log");
#endif
	MessageProc::StartLogging(LoggingPath);
	MessageProc::AddLog("----------------------------------------", MessageProc::LOG_TYPE_INFO);
	MessageProc::AddLog("------- CmdFreak service program -------", MessageProc::LOG_TYPE_INFO);
	StkPlSPrintf(LogBuf, 1024, "Service started  [Ver=%s, Build=%s %s]", SERVICE_VERSION, __DATE__, __TIME__);
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

	wchar_t IpAddr[256];
	int Port;

	// Load properties
	StkProperties* Prop = new StkProperties();
	if (Prop->GetProperties(L"sample.conf") == 0) {
		char IpAddrTmp[256];
		if (Prop->GetPropertyStr("servicehost", IpAddrTmp) != 0) {
			MessageProc::AddLog("servicehost is not found in sample.conf.", MessageProc::LOG_TYPE_FATAL);
			return -1;
		}
		StkPlSPrintf(LogBuf, 1024, "servicehost property = %s", IpAddrTmp);
		MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
		StkPlConvUtf8ToWideChar(IpAddr, 256, IpAddrTmp);

		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			MessageProc::AddLog("serviceport is not found in sample.conf.", MessageProc::LOG_TYPE_FATAL);
			return -1;
		}
		StkPlSPrintf(LogBuf, 1024, "serviceport property = %d", Port);
		MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

		MessageProc::AddLog("sample.conf is loaded.", MessageProc::LOG_TYPE_INFO);
	} else {
		MessageProc::AddLog("sample.conf is not found.", MessageProc::LOG_TYPE_FATAL);
		return -1;
	}

	// DataAccess instance
	DataAccess* DatAc = DataAccess::GetInstance();

	// Initialize data tables and start AutoSave
	if (DatAc->CreateCmdFreakTables() != 0) {
		MessageProc::AddLog("sample.dat is not found.", MessageProc::LOG_TYPE_FATAL);
		// Stop AutoSave
		DatAc->StopAutoSave();
		return -1;
	}
	int DbVersion = DatAc->GetDbVersion();
	if (DbVersion == -1) {
		DbVersion = DatAc->DbUpdate_NonVer_V3();
	}
	StkPlSPrintf(LogBuf, 1024, "sample.dat is loaded. (DB version = %d)", DbVersion);
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

	StkWebAppUm_AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTART), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTART), -1);
	MessageProc::AddLog("Main process has started.", MessageProc::LOG_TYPE_INFO);

	// Exec rest api
	CmdFreakRestApi(IpAddr, Port);

	StkWebAppUm_AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTOP), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTOP), -1);
	MessageProc::AddLog("Main process has ended.", MessageProc::LOG_TYPE_INFO);

	// Stop AutoSave
	DatAc->StopAutoSave();

	// Logging ends
	MessageProc::AddLog("Service ended", MessageProc::LOG_TYPE_INFO);
	MessageProc::StopLogging();

	return 0;
}
