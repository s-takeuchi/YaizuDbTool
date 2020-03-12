#include "ApiLogging.h"
#include "dataaccess.h"
#include "MyMsgProc.h"
#include "..\Global.h"
#
StkObject* ApiLogging::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t LogMsgTime[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGTIME];
	wchar_t LogMsgEn[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG];
	wchar_t LogMsgJa[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG];

	DataAccess* Dac = DataAccess::GetInstance();
	int NumOfRec = Dac->GetLogs(LogMsgTime, LogMsgEn, LogMsgJa);

	StkObject* ResObj = new StkObject(L"");
	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	if (!CheckCredentials(Token, YourName)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	AddCodeAndMsg(ResObj, 0, L"", L"");
	StkObject* DatObj = new StkObject(L"Data");
	for (int Loop = 0; Loop < NumOfRec; Loop++) {
		StkObject* Log4Add = new StkObject(L"Log");
		Log4Add->AppendChildElement(new StkObject(L"Time", LogMsgTime[Loop]));
		Log4Add->AppendChildElement(new StkObject(L"MsgEn", LogMsgEn[Loop]));
		Log4Add->AppendChildElement(new StkObject(L"MsgJa", LogMsgJa[Loop]));
		DatObj->AppendChildElement(Log4Add);
	}
	ResObj->AppendChildElement(DatObj);

	*ResultCode = 200;

	return ResObj;
}
