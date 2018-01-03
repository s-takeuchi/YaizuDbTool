#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "MyMsgProc.h"
#include "ApiGetRecord.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkStringParser.h"

StkObject* ApiGetRecord::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));
	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);

	StkObject* RecObj = new StkObject(_T(""));
	if (StrStr(UrlPath, _T("?query="))) {
		DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
		TCHAR Dummy[256];
		TCHAR TableName[256];
		StkStringParser::ParseInto2Params(UrlPath, _T("$?query=$"), _T('$'), Dummy, 256, TableName, 256);
		StkObject* DummyObj = new StkObject(_T("Dummy"));
		int NumOfCols = Da->GetColumnInfoByTableName((SQLTCHAR*)TableName, DummyObj, StateMsg, Msg, 1024);
		delete DummyObj;
		int NumOfRecs = Da->GetRecordsByTableName(TableName, NumOfCols, DatObj, StateMsg, Msg, 1024);
		OdbcManager::GetInstance()->DeleteAccessorObject(Da);
		ResObj->AppendChildElement(DatObj);
		*ResultCode = 200;
	}

	return ResObj;
}
