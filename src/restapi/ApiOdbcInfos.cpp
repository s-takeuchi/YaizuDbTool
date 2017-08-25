#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "ApiOdbcInfos.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"

StkObject* ApiOdbcInfos::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));

	if (StrStr(UrlPath, _T("?query=default"))) {
		SQLTCHAR ConnStrPostgreSql[Global::MAX_PARAM_LENGTH];
		SQLTCHAR ConnStrMariaDb[Global::MAX_PARAM_LENGTH];
		SQLTCHAR ConnStrMySql[Global::MAX_PARAM_LENGTH];

		DbAccessor* DaPostgreSql = NULL;
		DbAccessor* DaMariaDb    = NULL;
		DbAccessor* DaMySql      = NULL;

		DaPostgreSql = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::POSTGRESQL_ACCESSOR);
		DaMariaDb    = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::MARIADB_ACCESSOR);
		DaMySql      = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::MYSQL_ACCESSOR);

		DaPostgreSql->GetDefaultConnStr(ConnStrPostgreSql);
		DaMariaDb->GetDefaultConnStr(ConnStrMariaDb);
		DaMySql->GetDefaultConnStr(ConnStrMySql);

		OdbcManager::GetInstance()->DeleteAccessorObject(DaPostgreSql);
		OdbcManager::GetInstance()->DeleteAccessorObject(DaMariaDb);
		OdbcManager::GetInstance()->DeleteAccessorObject(DaMySql);

		StkObject* DatObjPostgreSql = new StkObject(_T("OdbcInfo"));
		DatObjPostgreSql->AppendChildElement(new StkObject(_T("DbType"), _T("PostgreSQL")));
		DatObjPostgreSql->AppendChildElement(new StkObject(_T("ConnStr"), ConnStrPostgreSql));
		DatObj->AppendChildElement(DatObjPostgreSql);

		StkObject* DatObjMariaDbSql = new StkObject(_T("OdbcInfo"));
		DatObjMariaDbSql->AppendChildElement(new StkObject(_T("DbType"), _T("MariaDB")));
		DatObjMariaDbSql->AppendChildElement(new StkObject(_T("ConnStr"), ConnStrMariaDb));
		DatObj->AppendChildElement(DatObjMariaDbSql);

		StkObject* DatObjMySql = new StkObject(_T("OdbcInfo"));
		DatObjMySql->AppendChildElement(new StkObject(_T("DbType"), _T("MySQL")));
		DatObjMySql->AppendChildElement(new StkObject(_T("ConnStr"), ConnStrMySql));
		DatObj->AppendChildElement(DatObjMySql);
	}
	if (StrStr(UrlPath, _T("?query=configured"))) {
	}

	ResObj->AppendChildElement(DatObj);

	*ResultCode = 200;

	return ResObj;
}
