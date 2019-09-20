#include <windows.h>
#include "ApiGetSystem.h"

ApiGetSystem::ApiGetSystem()
{
	GetLocalTimeWStr();
}

void ApiGetSystem::GetLocalTimeWStr()
{
	SYSTEMTIME Systime;
	GetLocalTime(&Systime);
	wchar_t *Mon[] = {L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"};
	wsprintf(LocalTimeStr, L"%s %d %d %02d:%02d:%02d", Mon[Systime.wMonth - 1], Systime.wDay, Systime.wYear, Systime.wHour, Systime.wMinute, Systime.wSecond);
}

void ApiGetSystem::SetNumOfThreads(int Num)
{
	NumOfThreads = Num;
}

StkObject* ApiGetSystem::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	StkObject* ResObj = new StkObject(L"");
	AddCodeAndMsg(ResObj, 0, L"", L"");
	StkObject* DatObj = new StkObject(L"Data");
	DatObj->AppendChildElement(new StkObject(L"ProductName", L"CmdFreak"));
	DatObj->AppendChildElement(new StkObject(L"Version", L"1.1.0"));
	wchar_t BuildDate[32];
	wsprintf(BuildDate, L"%S %S", __DATE__, __TIME__);
	DatObj->AppendChildElement(new StkObject(L"BuildTime", BuildDate));
	DatObj->AppendChildElement(new StkObject(L"StartTime", LocalTimeStr));
	DatObj->AppendChildElement(new StkObject(L"NumOfThreads", NumOfThreads));
	ResObj->AppendChildElement(DatObj);
	*ResultCode = 200;

	return ResObj;
}
