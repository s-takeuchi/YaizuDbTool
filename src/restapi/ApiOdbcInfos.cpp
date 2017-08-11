#include <tchar.h>
#include "ApiOdbcInfos.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"

StkObject* ApiOdbcInfos::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	SQLTCHAR ConnStrPostgreSql[Global::MAX_PARAM_LENGTH];
	SQLTCHAR ConnStrMariaDb[Global::MAX_PARAM_LENGTH];

	DbAccessor* DaPostgreSql = NULL;
	DbAccessor* DaMariaDb    = NULL;

	DaPostgreSql = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::POSTGRESQL_ACCESSOR);
	DaMariaDb = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::MARIADB_ACCESSOR);

	DaPostgreSql->GetDefaultConnStr(ConnStrPostgreSql);
	DaMariaDb->GetDefaultConnStr(ConnStrMariaDb);

	OdbcManager::GetInstance()->DeleteAccessorObject(DaPostgreSql);
	OdbcManager::GetInstance()->DeleteAccessorObject(DaMariaDb);

	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));

	*ResultCode = 200;

	return ResObj;
}
