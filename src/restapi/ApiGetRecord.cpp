#include "MyMsgProc.h"
#include "ApiGetRecord.h"
#include "dataaccess.h"
#include "Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"

StkObject* ApiGetRecord::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");
	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	int YourId = -1;
	if (!CheckCredentials(Token, YourName, &YourId)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	if (!StkPlWcsStr(UrlPath, L"?query=")) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
		return ResObj;
	}
	wchar_t Dummy1[512] = L"";
	wchar_t Dummy2[512] = L"";
	wchar_t TableName[512] = L"";
	wchar_t TableNameAc[512] = L"";
	wchar_t SortColumnName[512] = L"";
	wchar_t SortColumnNameAc[512] = L"";
	wchar_t SortOrder[512] = L"";
	wchar_t Limit[512] = L"";
	int LimitInt = -1;
	wchar_t Offset[512] = L"";
	int OffsetInt = -1;

	if (StkPlWcsStr(UrlPath, L"query=") != NULL) {
		if (StkStringParser::ParseInto3Params(UrlPath, L"$query=$&$", L'$', Dummy1, 512, TableName, 512, Dummy2, 512) != 1) {
			StkStringParser::ParseInto2Params(UrlPath, L"$query=$", L'$', Dummy1, 512, TableName, 512);
		}
		DecodeURL(TableName, 512, TableNameAc, 512);
	}
	if (StkPlWcsStr(UrlPath, L"sort=") != NULL) {
		if (StkStringParser::ParseInto3Params(UrlPath, L"$sort=$&$", L'$', Dummy1, 512, SortColumnName, 512, Dummy2, 512) != 1) {
			StkStringParser::ParseInto2Params(UrlPath, L"$sort=$", L'$', Dummy1, 512, SortColumnName, 512);
		}
		DecodeURL(SortColumnName, 512, SortColumnNameAc, 512);
	}
	if (StkPlWcsStr(UrlPath, L"sortOrder=") != NULL) {
		if (StkStringParser::ParseInto3Params(UrlPath, L"$sortOrder=$&$", L'$', Dummy1, 512, SortOrder, 512, Dummy2, 512) != 1) {
			StkStringParser::ParseInto2Params(UrlPath, L"$sortOrder=$", L'$', Dummy1, 512, SortOrder, 512);
		}
	}
	if (StkPlWcsStr(UrlPath, L"limit=") != NULL) {
		if (StkStringParser::ParseInto3Params(UrlPath, L"$limit=$&$", L'$', Dummy1, 512, Limit, 512, Dummy2, 512) != 1) {
			StkStringParser::ParseInto2Params(UrlPath, L"$limit=$", L'$', Dummy1, 512, Limit, 512);
		}
		LimitInt = StkPlWcsToL(Limit);
	}
	if (StkPlWcsStr(UrlPath, L"offset=") != NULL) {
		if (StkStringParser::ParseInto3Params(UrlPath, L"$offset=$&$", L'$', Dummy1, 512, Offset, 512, Dummy2, 512) != 1) {
			StkStringParser::ParseInto2Params(UrlPath, L"$offset=$", L'$', Dummy1, 512, Offset, 512);
		}
		OffsetInt = StkPlWcsToL(Offset);
	}

	wchar_t StateMsg[10];
	wchar_t Msg[1024];

	// Get table names
	DbAccessor* DaTableName = OdbcManager::GetInstance()->CreateAccessorObject();
	StkObject* TableNameObj = new StkObject(L"TableName");
	StkObject* TargetTableNameObj = new StkObject(L"Name", TableNameAc);
	DaTableName->GetTables(TableNameObj, StateMsg, Msg);
	bool TblFound = false;
	if (TableNameObj->Contains(TargetTableNameObj) != NULL) {
		TblFound = true;
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
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject();
	StkObject* ColumnObj = new StkObject(L"Column");
	int NumOfCols = Da->GetColumnInfoByTableName(TableNameAc, ColumnObj, StateMsg, Msg);
	delete ColumnObj;
	StkObject* DatObj = new StkObject(L"Data");
	int NumOfRecs = Da->GetRecordsByTableName(TableNameAc, NumOfCols, DatObj, SortColumnNameAc, SortOrder, LimitInt, OffsetInt, StateMsg, Msg);
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
