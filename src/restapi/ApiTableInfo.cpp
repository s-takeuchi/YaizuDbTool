#include <tchar.h>
#include "ApiTableInfo.h"
#include "dataaccess.h"
#include "DbAccessor.h"
#include "OdbcManager.h"

StkObject* ApiTableInfo::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));
	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));

	StkObject* TableInfoObj = new StkObject(_T("TableInfo"));
	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	Da->GetTables(TableInfoObj, StateMsg, Msg, 1024);
	OdbcManager::GetInstance()->DeleteAccessorObject(Da);
	if (TableInfoObj->GetFirstChildElement() != NULL) {
		ResObj->AppendChildElement(TableInfoObj);
	}

	*ResultCode = 200;

	return ResObj;
}
