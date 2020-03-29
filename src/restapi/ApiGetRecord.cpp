#include "MyMsgProc.h"
#include "ApiGetRecord.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkStringParser.h"

StkObject* ApiGetRecord::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");
	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	if (!CheckCredentials(Token, YourName)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	if (!StkPlWcsStr(UrlPath, L"?query=")) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}
	wchar_t Dummy[256] = L"";
	wchar_t TableName[768] = L"";
	wchar_t SortColumnName[768] = L"";
	wchar_t SortOrder[32] = L"";
	if (StkPlWcsStr(UrlPath, L"&") != NULL) {
		StkStringParser::ParseInto4Params(UrlPath, L"$?query=$&sort=$&sortOrder=$", L'$', Dummy, 256, TableName, 768, SortColumnName, 768, SortOrder, 32);
	} else {
		StkStringParser::ParseInto2Params(UrlPath, L"$?query=$", L'$', Dummy, 256, TableName, 768);
	}
	wchar_t TableNameAc[768] = L"";
	DecodeURL(TableName, TableNameAc);
	wchar_t SortColumnNameAc[768] = L"";
	DecodeURL(SortColumnName, SortColumnNameAc);

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
	bool TblFound = FALSE;
	if (TableNameObj->Contains(TargetTableNameObj) != NULL) {
		TblFound = TRUE;
	}
	delete TargetTableNameObj;
	delete TableNameObj;
	OdbcManager::GetInstance()->DeleteAccessorObject(DaTableName);
	if (!TblFound) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_TABLE_NOT_EXIST, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_TABLE_NOT_EXIST), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_TABLE_NOT_EXIST));
		*ResultCode = 404;
		return ResObj;
	}

	// Get records
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	StkObject* ColumnObj = new StkObject(L"Column");
	int NumOfCols = Da->GetColumnInfoByTableName((SQLTCHAR*)TableNameAc, ColumnObj, StateMsg, Msg, 1024);
	delete ColumnObj;
	StkObject* DatObj = new StkObject(L"Data");
	int NumOfRecs = Da->GetRecordsByTableName(TableNameAc, NumOfCols, DatObj, SortColumnNameAc, SortOrder, StateMsg, Msg, 1024);
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);

	if (SortColumnNameAc != NULL && *SortColumnNameAc != L'\0') {
		StkObject* SearchObj = new StkObject(L"Data");
		StkObject* FoundObj = DatObj->Contains(SearchObj);
		StkObject* NewObj = new StkObject(L"Sort");
		NewObj->AppendChildElement(new StkObject(L"Target", SortColumnNameAc));
		NewObj->AppendChildElement(new StkObject(L"Order", SortOrder));
		FoundObj->AppendChildElement(NewObj);
		delete SearchObj;
	}

	AddCodeAndMsg(ResObj, 0, L"", L"");
	ResObj->AppendChildElement(DatObj);
	*ResultCode = 200;

	return ResObj;
}
