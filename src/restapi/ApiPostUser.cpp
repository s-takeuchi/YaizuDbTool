#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "MyMsgProc.h"
#include "ApiPostUser.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiPostUser::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");

	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	if (!CheckCredentials(Token, YourName)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

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
	int Role = -1;
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
			StkPlWcsCpy(Password, Global::MAXLEN_OF_PASSWORD, CurObj->GetStringValue());
		} else if (StkPlWcsCmp(CurObj->GetName(), L"Role") == 0) {
			Role = CurObj->GetIntValue();
		}
		CurObj = CurObj->GetNext();
	}
	if (StkPlWcsCmp(Name, L"") == 0 || Role == -1) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}

	int TmpId = -1;
	wchar_t TmpName[Global::MAXLEN_OF_USERNAME] = L"";
	wchar_t TmpPassword[Global::MAXLEN_OF_PASSWORD] = L"";
	int TmpRole = -1;
	if (Id != -1 && DataAccess::GetInstance()->GetTargetUserById(Id, TmpName, TmpPassword, &TmpRole) == false) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_USER_DOES_NOT_EXIST, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_USER_DOES_NOT_EXIST), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_USER_DOES_NOT_EXIST));
		*ResultCode = 400;
		return ResObj;
	}
	// In case user information yourself is cahnged.
	if (StkPlWcsCmp(YourName, TmpName) == 0) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_CANNOT_MODIFY_YOUR_INFO, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_CANNOT_MODIFY_YOUR_INFO), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_CANNOT_MODIFY_YOUR_INFO));
		*ResultCode = 400;
		return ResObj;
	}
	// User name is changed, and the name already exists.
	if (StkPlWcsCmp(TmpName, Name) != 0 && DataAccess::GetInstance()->GetTargetUserByName(Name, &TmpId, TmpPassword, &TmpRole) == true) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_SAME_USER_NAME_EXIST, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_SAME_USER_NAME_EXIST), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_SAME_USER_NAME_EXIST));
		*ResultCode = 400;
		return ResObj;
	}

	AddCodeAndMsg(ResObj, 0, L"", L"");
	*ResultCode = 200;
	if (Id == -1) {
		DataAccess::GetInstance()->AddUser(Name, Role, Password);
		wchar_t LogBufEng[512] = L"";
		wchar_t LogBufJpn[512] = L"";
		StkPlSwPrintf(LogBufEng, 512, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_USER_ADD), Name);
		StkPlSwPrintf(LogBufJpn, 512, MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_USER_ADD), Name);
		DataAccess::GetInstance()->AddLogMsg(LogBufEng, LogBufJpn);
	} else {
		DataAccess::GetInstance()->UpdateUser(Id, Name, Role, Password);
		wchar_t LogBufEng[512] = L"";
		wchar_t LogBufJpn[512] = L"";
		StkPlSwPrintf(LogBufEng, 512, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_USER_EDIT), Name);
		StkPlSwPrintf(LogBufJpn, 512, MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_USER_EDIT), Name);
		DataAccess::GetInstance()->AddLogMsg(LogBufEng, LogBufJpn);
	}

	return ResObj;
}
