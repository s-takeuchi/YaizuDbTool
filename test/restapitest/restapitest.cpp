#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

void TestGetSystem(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("GetSystem ... ");
	int ResultCode = 0;
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/system/", NULL, &ResultCode);
	if (ResObj == NULL) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkObject* Dat = ResObj->GetFirstChildElement();
	bool ErrFlag = true;
	while (Dat) {
		if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0 && Dat->GetIntValue() != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
			Dat = Dat->GetFirstChildElement();
		}
		if (StkPlWcsCmp(Dat->GetName(), L"ProductName") == 0 && StkPlWcsCmp(Dat->GetStringValue(), L"CmdFreak") == 0) {
			ErrFlag = false;
		}
		if (StkPlWcsCmp(Dat->GetName(), L"ProductName") == 0 && StkPlWcsCmp(Dat->GetStringValue(), L"CmdFreak") != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		Dat = Dat->GetNext();
	}
	delete ResObj;
	if (ErrFlag) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\r\n");
}

void TestGetOdbcInfo(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("GetOdbcInfo ... ");
	int ResultCode = 0;
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/odbcinfo/?query=default", NULL, &ResultCode);
	if (ResObj == NULL) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkObject* Dat = ResObj->GetFirstChildElement();
	bool ErrFlag = true;
	while (Dat) {
		if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0 && Dat->GetIntValue() != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
			if (StkPlWcsCmp(Dat->GetFirstChildElement()->GetName(), L"OdbcInfo") == 0) {
				wchar_t OdbcInfoStr[1024];
				Dat->ToJson(OdbcInfoStr, 1024);
				StkObject* PostgreObj = new StkObject(L"DbType", L"PostgreSQL");
				StkObject* MySqlObj = new StkObject(L"DbType", L"MySQL");
				StkObject* MariaDbObj = new StkObject(L"DbType", L"MariaDB");
				if (!Dat->Contains(PostgreObj) || !Dat->Contains(MySqlObj) || !Dat->Contains(MariaDbObj)) {
					StkPlPrintf("[NG]\r\n");
					StkPlExit(1);
				}
				delete PostgreObj;
				delete MySqlObj;
				delete MariaDbObj;
				ErrFlag = false;
			} else {
				StkPlPrintf("[NG]\r\n");
				StkPlExit(1);
			}
		}
		Dat = Dat->GetNext();
	}
	delete ResObj;
	if (ErrFlag) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\r\n");
}

void TestPostOperationStop(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("PostOperationStop ... ");
	int ResultCode = 0;
	StkObject* ReqObj = new StkObject(L"Operation", L"Stop");
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/service/", ReqObj, &ResultCode);
	StkPlPrintf("[OK]\r\n");
}

int main(int Argc, char* Argv[])
{
	StkWebAppSend* StkWebAppSendObj = new StkWebAppSend(10, L"localhost", 2060);
	TestGetSystem(StkWebAppSendObj);
	TestGetOdbcInfo(StkWebAppSendObj);
	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
