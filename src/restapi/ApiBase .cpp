#include <windows.h>
#include <tchar.h>
#include "ApiBase.h"

void ApiBase::AddCodeAndMsg(StkObject* StkObj, int Code, TCHAR* MsgEng, TCHAR* MsgJpn)
{
	StkObj->AppendChildElement(new StkObject(_T("Code"), Code));
	StkObj->AppendChildElement(new StkObject(_T("MsgEng"), MsgEng));
	StkObj->AppendChildElement(new StkObject(_T("MsgJpn"), MsgJpn));
}

void ApiBase::GetLocalTimeWStr(TCHAR LocalTimeStr[32])
{
	SYSTEMTIME Systime;
	GetLocalTime(&Systime);
	TCHAR *Mon[] = {_T("Jan"), _T("Feb"), _T("Mar"), _T("Apr"), _T("May"), _T("Jun"), _T("Jul"), _T("Aug"), _T("Sep"), _T("Oct"), _T("Nov"), _T("Dec")};
	wsprintf(LocalTimeStr, _T("%s %d %d %02d:%02d:%02d.%03d"), Mon[Systime.wMonth - 1], Systime.wDay, Systime.wYear, Systime.wHour, Systime.wMinute, Systime.wSecond, Systime.wMilliseconds);
}

void ApiBase::PrintRequest(int Method, TCHAR UrlPath[128])
{
	TCHAR StrMethod[32];
	switch (Method) {
	case STKWEBAPP_METHOD_UNDEFINED:
		lstrcpy(StrMethod, _T("Undifined")); break;
	case STKWEBAPP_METHOD_GET:
		lstrcpy(StrMethod, _T("Get")); break;
	case STKWEBAPP_METHOD_HEAD:
		lstrcpy(StrMethod, _T("Head")); break;
	case STKWEBAPP_METHOD_POST:
		lstrcpy(StrMethod, _T("Post")); break;
	case STKWEBAPP_METHOD_PUT:
		lstrcpy(StrMethod, _T("Put")); break;
	case STKWEBAPP_METHOD_DELETE:
		lstrcpy(StrMethod, _T("Delete")); break;
	default:
		lstrcpy(StrMethod, _T("Invalid")); break;
	}
	TCHAR LocalTimeStr[32];
	GetLocalTimeWStr(LocalTimeStr);
	DWORD ThId = GetCurrentThreadId();
	wprintf(_T("%s  [%06x]  %s %s\r\n"), LocalTimeStr, ThId, StrMethod, UrlPath);
}

void ApiBase::PrintResponse(int ResultCode)
{
	TCHAR LocalTimeStr[32];
	GetLocalTimeWStr(LocalTimeStr);
	DWORD ThId = GetCurrentThreadId();
	wprintf(_T("%s  [%06x]  %d\r\n"), LocalTimeStr, ThId, ResultCode);
}

StkObject* ApiBase::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	PrintRequest(Method, UrlPath);

	StkObject* RetObj = ExecuteImpl(ReqObj, Method, UrlPath, ResultCode, Locale);

	PrintResponse(*ResultCode);

	return RetObj;
}
