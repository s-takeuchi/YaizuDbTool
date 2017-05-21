#include <tchar.h>
#include "Sample_Elem1.h"

Sample_Elem1::Sample_Elem1()
{
	GetLocalTimeWStr();
}

void Sample_Elem1::GetLocalTimeWStr()
{
	SYSTEMTIME Systime;
	GetLocalTime(&Systime);
	TCHAR *Mon[] = {_T("Jan"), _T("Feb"), _T("Mar"), _T("Apr"), _T("May"), _T("Jun"), _T("Jul"), _T("Aug"), _T("Sep"), _T("Oct"), _T("Nov"), _T("Dec")};
	wsprintf(LocalTimeStr, _T("%s %d %d %02d:%02d:%02d"), Mon[Systime.wMonth - 1], Systime.wDay, Systime.wYear, Systime.wHour, Systime.wMinute, Systime.wSecond);
}

StkObject* Sample_Elem1::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode)
{
	StkObject* ResObj = new StkObject(_T("Res"));
	ResObj->AppendChildElement(new StkObject(_T("Code"), 0));
	ResObj->AppendChildElement(new StkObject(_T("MsgEng"), _T("")));
	ResObj->AppendChildElement(new StkObject(_T("MsgJpn"), _T("")));
	StkObject* DatObj = new StkObject(_T("Data"));
	DatObj->AppendChildElement(new StkObject(_T("ProductName"), _T("CmdFreak")));
	DatObj->AppendChildElement(new StkObject(_T("Version"), _T("1.1.0")));
	TCHAR BuildDate[32];
	wsprintf(BuildDate, _T("%S %S"), __DATE__, __TIME__);
	DatObj->AppendChildElement(new StkObject(_T("BuildTime"), BuildDate));
	DatObj->AppendChildElement(new StkObject(_T("StartTime"), LocalTimeStr));
	ResObj->AppendChildElement(DatObj);

	return ResObj;
}
