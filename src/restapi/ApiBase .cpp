#include <windows.h>
#include <tchar.h>
#include "ApiBase.h"

void ApiBase::AddCodeAndMsg(StkObject* StkObj, int Code, TCHAR* MsgEng, TCHAR* MsgJpn)
{
	StkObj->AppendChildElement(new StkObject(_T("Code"), Code));
	StkObj->AppendChildElement(new StkObject(_T("MsgEng"), MsgEng));
	StkObj->AppendChildElement(new StkObject(_T("MsgJpn"), MsgJpn));
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
	wprintf(_T("%s %s\r\n"), StrMethod, UrlPath);
}
