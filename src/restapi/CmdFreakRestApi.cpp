#include "../../../YaizuComLib/src/stkpl\StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/StkProperties.h"
#include "../../../YaizuComLib/src/stkthread/stkthread.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebApp.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppExec.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "dataaccess.h"
#include "MyMsgProc.h"
#include "ApiGetSystem.h"
#include "ApiOdbcInfo.h"
#include "ApiGetTableInfo.h"
#include "ApiGetRecord.h"
#include "ApiGetRecCount.h"
#include "ApiFilterInfo.h"

void CmdFreakRestApi(wchar_t* IpAddr, int Port, int SendBufSize)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 7, IpAddr, Port);
	Soc->SetSendBufSize(SendBufSize);

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

	StkWebAppUm_AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTART), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTART));

	// Exec rest api
	CmdFreakRestApi(IpAddr, Port, SendBufSize);

	StkWebAppUm_AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SVCSTOP), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SVCSTOP));

	// Stop AutoSave
	DatAc->StopAutoSave();

	return 0;
}
