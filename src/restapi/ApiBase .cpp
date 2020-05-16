#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"
#include "..\Global.h"
#include "dataaccess.h"
#include "ApiBase.h"

void ApiBase::AddCodeAndMsg(StkObject* StkObj, int Code, wchar_t* MsgEng, wchar_t* MsgJpn)
{
	StkObj->AppendChildElement(new StkObject(L"Code", Code));
	StkObj->AppendChildElement(new StkObject(L"MsgEng", MsgEng));
	StkObj->AppendChildElement(new StkObject(L"MsgJpn", MsgJpn));
}

void ApiBase::PrintRequest(unsigned int Identifier, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH])
{
	wchar_t StrMethod[32];
	switch (Method) {
	case STKWEBAPP_METHOD_UNDEFINED:
		StkPlWcsCpy(StrMethod, 32, L"Undifined"); break;
	case STKWEBAPP_METHOD_GET:
		StkPlWcsCpy(StrMethod, 32, L"Get"); break;
	case STKWEBAPP_METHOD_HEAD:
		StkPlWcsCpy(StrMethod, 32, L"Head"); break;
	case STKWEBAPP_METHOD_POST:
		StkPlWcsCpy(StrMethod, 32, L"Post"); break;
	case STKWEBAPP_METHOD_PUT:
		StkPlWcsCpy(StrMethod, 32, L"Put"); break;
	case STKWEBAPP_METHOD_DELETE:
		StkPlWcsCpy(StrMethod, 32, L"Delete"); break;
	default:
		StkPlWcsCpy(StrMethod, 32, L"Invalid"); break;
	}
	wchar_t LocalTimeStr[64];
	StkPlGetWTimeInOldFormat(LocalTimeStr, true);
	StkPlWPrintf(L"%08x| %ls   %ls %ls\r\n", Identifier, LocalTimeStr, StrMethod, UrlPath);
}

void ApiBase::PrintResponse(unsigned int Identifier, int ResultCode)
{
	wchar_t LocalTimeStr[64];
	StkPlGetWTimeInOldFormat(LocalTimeStr, true);
	StkPlWPrintf(L"%08x| %ls   %d\r\n", Identifier, LocalTimeStr, ResultCode);
}

void ApiBase::DebugObject(StkObject* StkObj)
{
	wchar_t DebugStr[65536];
	StkObj->ToJson(DebugStr, 65536);
	StkPlWPrintf(L"%ls\r\n", DebugStr);
}

void ApiBase::DecodeURL(wchar_t UrlIn[StkWebAppExec::URL_PATH_LENGTH], wchar_t UrlOut[StkWebAppExec::URL_PATH_LENGTH])
{
	char TmpUrlBc[StkWebAppExec::URL_PATH_LENGTH] = "";
	char TmpUrlAc[StkWebAppExec::URL_PATH_LENGTH] = "";
	StkPlConvWideCharToUtf8(TmpUrlBc, StkWebAppExec::URL_PATH_LENGTH, UrlIn);
	int TmpUrlBcLen = (int)StkPlStrLen(TmpUrlBc);
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
	StkPlConvUtf8ToWideChar(UrlOut, StkWebAppExec::URL_PATH_LENGTH, TmpUrlAc);
	return;
}

bool ApiBase::CheckCredentials(wchar_t* Token, wchar_t* Name)
{
	if (Token == NULL || *Token == L'\0') {
		return false;
	}

	wchar_t TmpName[Global::MAXLEN_OF_USERNAME] = L"";
	wchar_t TmpPassword[Global::MAXLEN_OF_PASSWORD] = L"";
	StkStringParser::ParseInto2Params(Token, L"# #", L'#', TmpName, Global::MAXLEN_OF_USERNAME, TmpPassword, Global::MAXLEN_OF_PASSWORD);
	if (TmpName == NULL || TmpPassword == NULL || *TmpName == L'\0' || *TmpPassword == L'\0') {
		return false;
	}

	int Id = 0;
	wchar_t Password[Global::MAXLEN_OF_PASSWORD];
	int Role = 0;
	bool Ret = DataAccess::GetInstance()->GetTargetUserByName(TmpName, &Id, Password, &Role);
	if (Ret == false) {
		return false;
	}

	if (StkPlWcsCmp(TmpPassword, Password) == 0) {
		if (Name != NULL) {
			StkPlWcsCpy(Name, Global::MAXLEN_OF_USERNAME, TmpName);
		}
		return true;
	} else {
		return false;
	}
}

bool ApiBase::IsAdminUser(wchar_t* Token)
{
	int UserId = 0;
	wchar_t UserName[Global::MAXLEN_OF_USERNAME] = L"";
	wchar_t UserPassword[Global::MAXLEN_OF_PASSWORD] = L"";
	int Role = 0;
	if (!CheckCredentials(Token, UserName)) {
		return false;
	}
	DataAccess::GetInstance()->GetTargetUserByName(UserName, &UserId, UserPassword, &Role);
	if (Role != 0) {
		return false;
	}
	return true;
}

StkObject* ApiBase::Execute(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t* HttpHeader)
{
	static unsigned int Identifier = 0;
	Identifier++;
	unsigned int LocalId = Identifier;
	PrintRequest(LocalId, Method, UrlPath);

	wchar_t Locale[3] = L"";;
	StkStringParser::ParseInto2Params(HttpHeader, L"#Accept-Language: #", L'#', NULL, 0, Locale, 3);

	wchar_t Token[256] = L"";
	StkStringParser::ParseInto4Params(HttpHeader, L"#Authorization:#Bearer #\r\n#", L'#', NULL, 0, NULL, 0, Token, 256, NULL, 0);

	StkObject* RetObj = ExecuteImpl(ReqObj, Method, UrlPath, ResultCode, Locale, Token);

	PrintResponse(LocalId, *ResultCode);

	return RetObj;
}
