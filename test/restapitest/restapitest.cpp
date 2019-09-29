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
		StkPlExit(0);
	}
	StkObject* Dat = ResObj->GetFirstChildElement();
	while (Dat) {
		if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0 && Dat->GetIntValue() != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(0);
		}
		if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
			Dat = Dat->GetFirstChildElement();
		}
		if (StkPlWcsCmp(Dat->GetName(), L"ProductName") == 0 && StkPlWcsCmp(Dat->GetStringValue(), L"CmdFreak") != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(0);
		}
		Dat = Dat->GetNext();
	}
	delete ResObj;
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
	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
