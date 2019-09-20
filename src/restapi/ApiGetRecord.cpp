#include <windows.h>
#include <shlwapi.h>
#include "MyMsgProc.h"
#include "ApiGetRecord.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkStringParser.h"

StkObject* ApiGetRecord::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	StkObject* ResObj = new StkObject(L"");

	if (!StrStr(UrlPath, L"?query=")) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}
	wchar_t Dummy[256];
	wchar_t TableName[768];
	StkStringParser::ParseInto2Params(UrlPath, L"$?query=$", L'$', Dummy, 256, TableName, 768);
	wchar_t TableNameAc[768];
	DecodeURL(TableName, TableNameAc);

	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);

	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	// Get table names
	DbAccessor* DaTableName = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	StkObject* TableNameObj = new StkObject(L"TableName");
	StkObject* TargetTableNameObj = new StkObject(L"Name", TableNameAc);
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
	StkObject* ColumnObj = new StkObject(L"Column");
	int NumOfCols = Da->GetColumnInfoByTableName((SQLTCHAR*)TableNameAc, ColumnObj, StateMsg, Msg, 1024);
	delete ColumnObj;
	StkObject* DatObj = new StkObject(L"Data");
	int NumOfRecs = Da->GetRecordsByTableName(TableNameAc, NumOfCols, DatObj, StateMsg, Msg, 1024);
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);

	AddCodeAndMsg(ResObj, 0, L"", L"");
	ResObj->AppendChildElement(DatObj);
	*ResultCode = 200;

	return ResObj;
}
