#include "..\..\..\YaizuComLib\src\stkpl\StkPl.h"
#include "MyMsgProc.h"
#include "..\Global.h"
#include "ApiGetSystem.h"

ApiGetSystem::ApiGetSystem()
{
	GetLocalTimeWStr();
}

void ApiGetSystem::GetLocalTimeWStr()
{
	int Year, MonA, Day, Hour, Min, Sec;
	StkPlGetTime(&Year, &MonA, &Day, &Hour, &Min, &Sec, true);
	wchar_t *Mon[] = {L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"};
	StkPlSwPrintf(LocalTimeStr, 32, L"%s %d %d %02d:%02d:%02d", Mon[MonA - 1], Day, Year, Hour, Min, Sec);
}

void ApiGetSystem::SetNumOfThreads(int Num)
{
	NumOfThreads = Num;
}

StkObject* ApiGetSystem::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");
	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	if (!CheckCredentials(Token, YourName)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	AddCodeAndMsg(ResObj, 0, L"", L"");
	StkObject* DatObj = new StkObject(L"Data");
	DatObj->AppendChildElement(new StkObject(L"ProductName", L"CmdFreak"));
	DatObj->AppendChildElement(new StkObject(L"Version", L"1.1.0"));
	wchar_t BuildDate[32];
	char BuildDateCh[32];
	StkPlSPrintf(BuildDateCh, 32, "%s  %s", __DATE__, __TIME__);
	StkPlConvUtf8ToWideChar(BuildDate, 32, BuildDateCh);
	DatObj->AppendChildElement(new StkObject(L"BuildTime", BuildDate));
	DatObj->AppendChildElement(new StkObject(L"StartTime", LocalTimeStr));
	DatObj->AppendChildElement(new StkObject(L"NumOfThreads", NumOfThreads));
	ResObj->AppendChildElement(DatObj);
	*ResultCode = 200;

	return ResObj;
}
