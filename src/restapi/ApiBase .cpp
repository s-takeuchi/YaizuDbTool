#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "ApiBase.h"

void ApiBase::AddCodeAndMsg(StkObject* StkObj, int Code, TCHAR* MsgEng, TCHAR* MsgJpn)
{
	StkObj->AppendChildElement(new StkObject(_T("Code"), Code));
	StkObj->AppendChildElement(new StkObject(_T("MsgEng"), MsgEng));
	StkObj->AppendChildElement(new StkObject(_T("MsgJpn"), MsgJpn));
}

void ApiBase::PrintRequest(int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH])
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
	TCHAR LocalTimeStr[64];
	StkPlGetWTimeInOldFormat(LocalTimeStr, true);
	DWORD ThId = GetCurrentThreadId();
	wprintf(_T("%s  [%06x]  %s %s\r\n"), LocalTimeStr, ThId, StrMethod, UrlPath);
}

void ApiBase::PrintResponse(int ResultCode)
{
	TCHAR LocalTimeStr[64];
	StkPlGetWTimeInOldFormat(LocalTimeStr, true);
	DWORD ThId = GetCurrentThreadId();
	wprintf(_T("%s  [%06x]  %d\r\n"), LocalTimeStr, ThId, ResultCode);
}

void ApiBase::DecodeURL(TCHAR UrlIn[StkWebAppExec::URL_PATH_LENGTH], TCHAR UrlOut[StkWebAppExec::URL_PATH_LENGTH])
{
	char TmpUrlBc[StkWebAppExec::URL_PATH_LENGTH] = "";
	BYTE TmpUrlAc[StkWebAppExec::URL_PATH_LENGTH];
	sprintf_s(TmpUrlBc, StkWebAppExec::URL_PATH_LENGTH, "%S", UrlIn);
	int TmpUrlBcLen = strlen(TmpUrlBc);
	int AcIndex = 0;
	for (int BcIndex = 0; BcIndex < TmpUrlBcLen; BcIndex++) {
		if (TmpUrlBc[BcIndex] == '%' && BcIndex + 2 < TmpUrlBcLen) {
			char Val = 0;
			if (TmpUrlBc[BcIndex + 1] >= '0' && TmpUrlBc[BcIndex + 1] <= '9') {
				Val += (TmpUrlBc[BcIndex + 1] - '0') * 16;
			}
			if (TmpUrlBc[BcIndex + 1] >= 'a' && TmpUrlBc[BcIndex + 1] <= 'f') {
				Val += (TmpUrlBc[BcIndex + 1] - 'a' + 10) * 16;
			}
			if (TmpUrlBc[BcIndex + 1] >= 'A' && TmpUrlBc[BcIndex + 1] <= 'F') {
				Val += (TmpUrlBc[BcIndex + 1] - 'A' + 10) * 16;
			}
			if (TmpUrlBc[BcIndex + 2] >= '0' && TmpUrlBc[BcIndex + 2] <= '9') {
				Val += (TmpUrlBc[BcIndex + 2] - '0');
			}
			if (TmpUrlBc[BcIndex + 2] >= 'a' && TmpUrlBc[BcIndex + 2] <= 'f') {
				Val += (TmpUrlBc[BcIndex + 2] - 'a' + 10);
			}
			if (TmpUrlBc[BcIndex + 2] >= 'A' && TmpUrlBc[BcIndex + 2] <= 'F') {
				Val += (TmpUrlBc[BcIndex + 2] - 'A' + 10);
			}
			TmpUrlAc[AcIndex] = Val;
			BcIndex += 2;
		} else if (TmpUrlBc[BcIndex] == '+') {
			TmpUrlAc[AcIndex] = ' ';
		} else {
			TmpUrlAc[AcIndex] = TmpUrlBc[BcIndex];
		}
		AcIndex++;
	}
	TmpUrlAc[AcIndex] = '\0';
	int WcSize = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)TmpUrlAc, -1, UrlOut, StkWebAppExec::URL_PATH_LENGTH);
	return;
}

StkObject* ApiBase::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, TCHAR Locale[3])
{
	PrintRequest(Method, UrlPath);

	StkObject* RetObj = ExecuteImpl(ReqObj, Method, UrlPath, ResultCode, Locale);

	PrintResponse(*ResultCode);

	return RetObj;
}
