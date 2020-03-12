#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "MyMsgProc.h"
#include "ApiDeleteUser.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkStringParser.h"

StkObject* ApiDeleteUser::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
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

	wchar_t Dummy[256] = L"";
	wchar_t UserIdStr[16] = L"";
	if (StkPlWcsStr(UrlPath, L"/")) {
		StkStringParser::ParseInto2Params(UrlPath, L"$user/$/", L'$', Dummy, 256, UserIdStr, 16);
	}
	if (UserIdStr == NULL || UserIdStr[0] == L'\0') {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}
	int UserId = StkPlWcsToL(UserIdStr);

	wchar_t TmpName[Global::MAXLEN_OF_USERNAME] = L"";
	wchar_t TmpPassword[Global::MAXLEN_OF_PASSWORD] = L"";
	int TmpRole = -1;
	if (DataAccess::GetInstance()->GetTargetUserById(UserId, TmpName, TmpPassword, &TmpRole) == false) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_USER_DOES_NOT_EXIST, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_USER_DOES_NOT_EXIST), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_USER_DOES_NOT_EXIST));
		*ResultCode = 400;
		return ResObj;
	}
	if (StkPlWcsCmp(YourName, TmpName) == 0) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_CANNOT_MODIFY_YOUR_INFO, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_CANNOT_MODIFY_YOUR_INFO), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_CANNOT_MODIFY_YOUR_INFO));
		*ResultCode = 400;
		return ResObj;
	}

	DataAccess::GetInstance()->DeleteUser(UserId);

	wchar_t LogBufEng[512] = L"";
	wchar_t LogBufJpn[512] = L"";
	StkPlSwPrintf(LogBufEng, 512, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_USER_DELETE), TmpName);
	StkPlSwPrintf(LogBufJpn, 512, MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_USER_DELETE), TmpName);
	DataAccess::GetInstance()->AddLogMsg(LogBufEng, LogBufJpn);

	AddCodeAndMsg(ResObj, 0, L"", L"");
	*ResultCode = 200;
	return ResObj;
}