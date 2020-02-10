#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "MyMsgProc.h"
#include "ApiPostUser.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiPostUser::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");

	if (!IsAdminUser(Token)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR));
		*ResultCode = 403;
		return ResObj;
	}

	if (ReqObj == NULL) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}
	int Id = -1;
	wchar_t Name[Global::MAXLEN_OF_USERNAME] = L"";
	wchar_t Password[Global::MAXLEN_OF_PASSWORD] = L"";
	int Role = 0;
	StkObject* CurObj = ReqObj->GetFirstChildElement();
	if (CurObj == NULL) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}
	while (CurObj) {
		if (StkPlWcsCmp(CurObj->GetName(), L"Id") == 0) {
			Id = CurObj->GetIntValue();
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Name") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= Global::MAXLEN_OF_USERNAME) {
				AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Name, Global::MAXLEN_OF_USERNAME, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Password") == 0) {
			if (StkPlWcsLen(CurObj->GetStringValue()) >= Global::MAXLEN_OF_PASSWORD) {
				AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG));
				*ResultCode = 400;
				return ResObj;
			}
			StkPlWcsCpy(Name, Global::MAXLEN_OF_PASSWORD, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Role") == 0) {
			Role = CurObj->GetIntValue();
		}
		CurObj = CurObj->GetNext();
	}
	DataAccess::GetInstance()->UpdateUser(Id, Name, Role);
	return NULL;
}
