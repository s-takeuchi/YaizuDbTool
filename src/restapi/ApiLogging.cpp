#include <tchar.h>
#include "ApiLogging.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiLogging::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	TCHAR LogMsgTime[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGTIME];
	TCHAR LogMsgEn[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG];
	TCHAR LogMsgJa[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG];

	DataAccess* Dac = DataAccess::GetInstance();
	int NumOfRec = Dac->GetLogs(LogMsgTime, LogMsgEn, LogMsgJa);

	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));
	for (int Loop = 0; Loop < NumOfRec; Loop++) {
		StkObject* Log4Add = new StkObject(_T("Log"));
		Log4Add->AppendChildElement(new StkObject(_T("Time"), LogMsgTime[Loop]));
		Log4Add->AppendChildElement(new StkObject(_T("MsgEn"), LogMsgEn[Loop]));
		Log4Add->AppendChildElement(new StkObject(_T("MsgJa"), LogMsgJa[Loop]));
		DatObj->AppendChildElement(Log4Add);
	}
	ResObj->AppendChildElement(DatObj);

	*ResultCode = 200;

	return ResObj;
}
