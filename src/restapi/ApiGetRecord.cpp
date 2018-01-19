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

StkObject* ApiGetRecord::ExecuteImpl(StkObject* ReqObj, int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));

	if (!StrStr(UrlPath, _T("?query="))) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}
	TCHAR Dummy[256];
	TCHAR TableName[768];
	StkStringParser::ParseInto2Params(UrlPath, _T("$?query=$"), _T('$'), Dummy, 256, TableName, 768);
	TCHAR TableNameAc[768];
	DecodeURL(TableName, TableNameAc);

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);

	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	// Get table names
	DbAccessor* DaTableName = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	StkObject* TableNameObj = new StkObject(_T("TableName"));
	StkObject* TargetTableNameObj = new StkObject(_T("Name"), TableNameAc);
	DaTableName->GetTables(TableNameObj, StateMsg, Msg, 1024);
	BOOL TblFound = FALSE;
	if (TableNameObj->Contains(TargetTableNameObj) != NULL) {
		TblFound = TRUE;
	}
	delete TargetTableNameObj;
	delete TableNameObj;
	OdbcManager::GetInstance()->DeleteAccessorObject(DaTableName);
	if (!TblFound) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_TABLE_NOT_EXIST, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_TABLE_NOT_EXIST),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_TABLE_NOT_EXIST));
		*ResultCode = 404;
		return ResObj;
	}

	// Get records
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	StkObject* ColumnObj = new StkObject(_T("Column"));
	int NumOfCols = Da->GetColumnInfoByTableName((SQLTCHAR*)TableNameAc, ColumnObj, StateMsg, Msg, 1024);
	delete ColumnObj;
	StkObject* DatObj = new StkObject(_T("Data"));
	int NumOfRecs = Da->GetRecordsByTableName(TableNameAc, NumOfCols, DatObj, StateMsg, Msg, 1024);
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);

	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	ResObj->AppendChildElement(DatObj);
	*ResultCode = 200;

	return ResObj;
}
