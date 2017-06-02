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
#include "ApiGetSystem.h"
#include "sample_elem2.h"
#include "sample_elem3.h"
#include "dataaccess.h"

void CmdFreakRestApi(TCHAR* IpAddr, int Port)
{
	int Ids[7] = {11, 12, 13, 14, 15, 16, 17};

	StkWebApp* Soc = new StkWebApp(Ids, 7, IpAddr, Port);

	ApiGetSystem* ApiGetSystemObj = new ApiGetSystem();
	int Add1 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/system/"), (StkWebAppExec*)ApiGetSystemObj);
	ApiGetSystemObj->SetNumOfThreads(7);
	Sample_Elem2* Test2Hndl = new Sample_Elem2();
	int Add2 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/aaa/xxx/"), (StkWebAppExec*)Test2Hndl);
	Sample_Elem3* Test3Hndl = new Sample_Elem3();
	int Add3 = Soc->AddReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/bbb/$/"), (StkWebAppExec*)Test3Hndl);

	////////// Main logic starts
	Soc->TheLoop();
	////////// Main logic ends

	int Del1 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/system/"));
	int Del2 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/aaa/xxx/"));
	int Del3 = Soc->DeleteReqHandler(StkWebApp::STKWEBAPP_METHOD_GET, _T("/api/bbb/$/"));

	delete Soc;
}

int main(int Argc, char* Argv[])
{
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
			printf("servicehost property is not found.\r\n");
			return -1;
		}
		printf("servicehost property = %s\r\n", IpAddrTmp);
		wsprintf(IpAddr, _T("%S"), IpAddrTmp);
		if (Prop->GetPropertyInt("serviceport", &Port) != 0) {
			printf("serviceport property is not found.\r\n");
			return -1;
		}
		printf("serviceport property = %d\r\n", Port);
	} else {
		printf("stkwebapp.conf is not found.\r\n");
		return -1;
	}

	// DataAccess instance
	DataAccess* DatAc = DataAccess::GetInstance();

	// Initialize data tables and start AutoSave
	if (DatAc->CreateCmdFreakTables() != 0) {
		printf("data file does not found.\r\n");
		return -1;
	}

	// Exec rest api
	CmdFreakRestApi(IpAddr, Port);

	// Stop AutoSave
	DatAc->StopAutoSave();

	return 0;
}
