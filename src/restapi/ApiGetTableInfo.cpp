#include <tchar.h>
#include <shlwapi.h>
#include "ApiGetTableInfo.h"
#include "dataaccess.h"
#include "DbAccessor.h"
#include "OdbcManager.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkStringParser.h"

StkObject* ApiGetTableInfo::ExecuteImpl(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	TCHAR Dummy[256] = _T("");
	TCHAR TableName[256] = _T("");
	if (StrStr(UrlPath, _T("?query="))) {
		StkStringParser::ParseInto2Params(UrlPath, _T("$?query=$"), _T('$'), Dummy, 256, TableName, 256);
	}

	StkObject* ResObj = new StkObject(_T(""));
	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	Da->GetTables(DatObj, StateMsg, Msg, 1024);

	// If ?query=TableName is specified...
	if (lstrcmp(TableName, _T("")) != 0) {
		StkObject* SearchTgtObj = new StkObject(_T("Name"), TableName);
		if (DatObj->Contains(SearchTgtObj)) {
			StkObject* TblInfObj = new StkObject(_T("TableInfo"));
			TblInfObj->AppendChildElement(SearchTgtObj);
			
			StkObject* DatObj2 = new StkObject(_T("Data"));
			DatObj2->AppendChildElement(TblInfObj);

			Da->GetColumnInfoByTableName(TableName, TblInfObj, StateMsg, Msg, 1024);
			ResObj->AppendChildElement(DatObj2);
		} else {
			delete SearchTgtObj;
		}
		delete DatObj;
	} else {
		StkObject* TblObj = DatObj->GetFirstChildElement();
		while (TblObj) {
			if (lstrcmp(TblObj->GetName(), _T("TableInfo")) == 0) {
				StkObject* TblNameObj = TblObj->GetFirstChildElement();
				while (TblNameObj) {
					if (lstrcmp(TblNameObj->GetName(), _T("Name")) == 0) {
						Da->GetColumnInfoByTableName(TblNameObj->GetStringValue(), TblObj, StateMsg, Msg, 1024);
					}
					TblNameObj = TblNameObj->GetNext();
				}
			};
			TblObj = TblObj->GetNext();
		}
		ResObj->AppendChildElement(DatObj);
	}
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);

	*ResultCode = 200;

	return ResObj;
}
