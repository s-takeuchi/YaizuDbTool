#include <tchar.h>
#include <shlwapi.h>
#include "MyMsgProc.h"
#include "ApiGetTableInfo.h"
#include "dataaccess.h"
#include "DbAccessor.h"
#include "OdbcManager.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkStringParser.h"

StkObject* ApiGetTableInfo::ExecuteImpl(StkObject* ReqObj, int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, TCHAR Locale[3])
{
	TCHAR Dummy[256] = _T("");
	TCHAR TableName[768] = _T("");
	if (StrStr(UrlPath, _T("?query="))) {
		StkStringParser::ParseInto2Params(UrlPath, _T("$?query=$"), _T('$'), Dummy, 256, TableName, 768);
	}

	StkObject* ResObj = new StkObject(_T(""));
	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	StkObject* DatObj = new StkObject(_T("Data"));

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	Da->GetTables(DatObj, StateMsg, Msg, 1024);

	if (lstrcmp(TableName, _T("")) != 0) {
		TCHAR TableNameAc[768];
		DecodeURL(TableName, TableNameAc);

		// If ?query=TableName is specified...
		StkObject* SearchTgtObj = new StkObject(_T("Name"), TableNameAc);
		if (DatObj->Contains(SearchTgtObj)) {
			StkObject* TblInfObj = new StkObject(_T("TableInfo"));
			TblInfObj->AppendChildElement(SearchTgtObj);
			
			StkObject* DatObj2 = new StkObject(_T("Data"));
			DatObj2->AppendChildElement(TblInfObj);

			Da->GetColumnInfoByTableName(TableNameAc, TblInfObj, StateMsg, Msg, 1024);

			*ResultCode = 200;
			AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
			ResObj->AppendChildElement(DatObj2);
		} else {
			// The specified table is not found.
			*ResultCode = 404;
			AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_TABLE_NOT_EXIST, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_TABLE_NOT_EXIST),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_TABLE_NOT_EXIST));
			delete SearchTgtObj;
		}
		delete DatObj;
	} else {
		// If ?query=TableName is not specified...
		*ResultCode = 200;
		AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
		ResObj->AppendChildElement(DatObj);
	}
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);

	return ResObj;
}
