#include <tchar.h>
#include "ApiGetSystem.h"

ApiGetSystem::ApiGetSystem()
{
	GetLocalTimeWStr();
}

void ApiGetSystem::GetLocalTimeWStr()
{
	SYSTEMTIME Systime;
	GetLocalTime(&Systime);
	TCHAR *Mon[] = {_T("Jan"), _T("Feb"), _T("Mar"), _T("Apr"), _T("May"), _T("Jun"), _T("Jul"), _T("Aug"), _T("Sep"), _T("Oct"), _T("Nov"), _T("Dec")};
	wsprintf(LocalTimeStr, _T("%s %d %d %02d:%02d:%02d"), Mon[Systime.wMonth - 1], Systime.wDay, Systime.wYear, Systime.wHour, Systime.wMinute, Systime.wSecond);
}

void ApiGetSystem::SetNumOfThreads(int Num)
{
	NumOfThreads = Num;
}

StkObject* ApiGetSystem::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	PrintRequest(Method, UrlPath);

	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));
	DatObj->AppendChildElement(new StkObject(_T("ProductName"), _T("CmdFreak")));
	DatObj->AppendChildElement(new StkObject(_T("Version"), _T("1.1.0")));
	TCHAR BuildDate[32];
	wsprintf(BuildDate, _T("%S %S"), __DATE__, __TIME__);
	DatObj->AppendChildElement(new StkObject(_T("BuildTime"), BuildDate));
	DatObj->AppendChildElement(new StkObject(_T("StartTime"), LocalTimeStr));
	DatObj->AppendChildElement(new StkObject(_T("NumOfThreads"), NumOfThreads));
	ResObj->AppendChildElement(DatObj);
	*ResultCode = 200;

	return ResObj;
}
