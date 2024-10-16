#include "MyMsgProc.h"
#include "ApiGetTableInfo.h"
#include "dataaccess.h"
#include "DbAccessor.h"
#include "OdbcManager.h"
#include "../../../YaizuComLib/src/commonfunc/StkStringParser.h"

StkObject* ApiGetTableInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t Dummy[256] = L"";
	wchar_t TableName[768] = L"";
	if (StkPlWcsStr(UrlPath, L"?query=")) {
		StkStringParser::ParseInto2Params(UrlPath, L"$?query=$", L'$', Dummy, 256, TableName, 768);
	}

	StkObject* ResObj = new StkObject(L"");
	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	int YourId = -1;
	if (!CheckCredentials(Token, YourName, &YourId)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	StkObject* DatObj = new StkObject(L"Data");

	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	Da->GetTables(DatObj, StateMsg, Msg, 1024);

	if (StkPlWcsCmp(TableName, L"") != 0) {
		wchar_t TableNameAc[768];
		DecodeURL(TableName, 768, TableNameAc, 768);

		// If ?query=TableName is specified...
		StkObject* SearchTgtObj = new StkObject(L"Name", TableNameAc);
		if (DatObj->Contains(SearchTgtObj)) {
			StkObject* TblInfObj = new StkObject(L"TableInfo");
			TblInfObj->AppendChildElement(SearchTgtObj);
			
			StkObject* DatObj2 = new StkObject(L"Data");
			DatObj2->AppendChildElement(TblInfObj);

			Da->GetColumnInfoByTableName((SQLTCHAR*)TableNameAc, TblInfObj, StateMsg, Msg, 1024);

			*ResultCode = 200;
			AddCodeAndMsg(ResObj, 0, L"", L"");
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
		AddCodeAndMsg(ResObj, 0, L"", L"");
		ResObj->AppendChildElement(DatObj);
	}
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);

	return ResObj;
}
