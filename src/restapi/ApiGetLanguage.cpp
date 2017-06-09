#include <tchar.h>
#include "MyMsgProc.h"
#include "ApiGetLanguage.h"

StkObject* ApiGetLanguage::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));
	if (Locale == NULL || Locale[2] != '\0' || Locale[0] == '\0') {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_NO_CLIENTLOCALE, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_NO_CLIENTLOCALE),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_NO_CLIENTLOCALE));
		*ResultCode = 406;
	} else {
		AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
		StkObject* DatObj = new StkObject(_T("Data"));
		DatObj->AppendChildElement(new StkObject(_T("ClientLanguage"), Locale));
		ResObj->AppendChildElement(DatObj);
		*ResultCode = 200;
	}
	return ResObj;
}
