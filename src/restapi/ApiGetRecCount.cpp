#include "MyMsgProc.h"
#include "ApiGetRecCount.h"
#include "dataaccess.h"
#include "../Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"

StkObject* ApiGetRecCount::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
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
	wchar_t Dummy[256] = L"";
	wchar_t TableName[768] = L"";
	StkStringParser::ParseInto2Params(UrlPath, L"$?query=$", L'$', Dummy, 256, TableName, 768);
	wchar_t TableNameAc[768] = L"";
	DecodeURL(TableName, 768, TableNameAc, 768);

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
	int NumOfRecs = Da->GetNumOfRecords(TableNameAc, StateMsg, Msg, 1024);
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);

	StkObject* DataObj = new StkObject(L"Data");
	DataObj->AppendChildElement(new StkObject(L"NumOfRecords", NumOfRecs));

	AddCodeAndMsg(ResObj, 0, L"", L"");
	ResObj->AppendChildElement(DataObj);
	*ResultCode = 200;

	return ResObj;
}
