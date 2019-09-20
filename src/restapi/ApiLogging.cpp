#include "ApiLogging.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiLogging::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	wchar_t LogMsgTime[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGTIME];
	wchar_t LogMsgEn[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG];
	wchar_t LogMsgJa[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG];

	DataAccess* Dac = DataAccess::GetInstance();
	int NumOfRec = Dac->GetLogs(LogMsgTime, LogMsgEn, LogMsgJa);

	StkObject* ResObj = new StkObject(L"");
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
