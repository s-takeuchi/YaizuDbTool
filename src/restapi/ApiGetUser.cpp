#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "MyMsgProc.h"
#include "ApiGetUser.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiGetUser::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	int UserId = 0;
	wchar_t UserName[Global::MAXLEN_OF_USERNAME] = L"";
	wchar_t UserPassword[Global::MAXLEN_OF_PASSWORD] = L"";
	int Role = 0;
	StkObject* TmpObj = new StkObject(L"");
	if (!CheckCredentials(Token, UserName)) {
		AddCodeAndMsg(TmpObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 403;
		return TmpObj;
	}
	DataAccess::GetInstance()->GetTargetUserByName(UserName, &UserId, UserPassword, &Role);
	if (StkPlWcsStr(UrlPath, L"?target=all") != NULL) {
		if (Role != 0) {
			AddCodeAndMsg(TmpObj, MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR));
			*ResultCode = 403;
			return TmpObj;
		}
		int AryUserId[Global::MAXNUM_OF_USERRECORDS];
		wchar_t AryUserName[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_USERNAME];
		wchar_t AryUserPassword[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_PASSWORD];
		int AryRole[Global::MAXNUM_OF_USERRECORDS];
		int Cnt = DataAccess::GetInstance()->GetTargetUsers(AryUserId, AryUserName, AryUserPassword, AryRole);
		for (int Loop = 0; Loop < Cnt; Loop++) {
			StkObject* TmpObjC = new StkObject(L"User");
			TmpObjC->AppendChildElement(new StkObject(L"Id", AryUserId[Loop]));
			TmpObjC->AppendChildElement(new StkObject(L"Name", AryUserName[Loop]));
			TmpObjC->AppendChildElement(new StkObject(L"Role", AryRole[Loop]));
			TmpObj->AppendChildElement(TmpObjC);
		}
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		*ResultCode = 200;
		return TmpObj;
	} else {
		StkObject* TmpObjC = new StkObject(L"User");
		TmpObjC->AppendChildElement(new StkObject(L"Id", UserId));
		TmpObjC->AppendChildElement(new StkObject(L"Name", UserName));
		TmpObjC->AppendChildElement(new StkObject(L"Role", Role));
		TmpObj->AppendChildElement(TmpObjC);
		AddCodeAndMsg(TmpObj, 0, L"", L"");
		*ResultCode = 200;
		return TmpObj;
	}
	return TmpObj;
}
