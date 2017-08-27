#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "ApiOdbcInfos.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"

StkObject* ApiOdbcInfos::GetOdbcInfos(TCHAR UrlPath[128], int* ResultCode)
{
	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));

	if (StrStr(UrlPath, _T("?query=default"))) {
		SQLTCHAR ConnStrPostgreSql[Global::MAX_PARAM_LENGTH];
		SQLTCHAR ConnStrMariaDb[Global::MAX_PARAM_LENGTH];
		SQLTCHAR ConnStrMySql[Global::MAX_PARAM_LENGTH];

		DbAccessor* DaMariaDb    = NULL;
		DbAccessor* DaPostgreSql = NULL;
		DbAccessor* DaMySql      = NULL;

		DaMariaDb    = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::MARIADB_ACCESSOR);
		DaPostgreSql = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::POSTGRESQL_ACCESSOR);
		DaMySql      = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::MYSQL_ACCESSOR);

		DaMariaDb->GetDefaultConnStr(ConnStrMariaDb);
		DaPostgreSql->GetDefaultConnStr(ConnStrPostgreSql);
		DaMySql->GetDefaultConnStr(ConnStrMySql);

		OdbcManager::GetInstance()->DeleteAccessorObject(DaMariaDb);
		OdbcManager::GetInstance()->DeleteAccessorObject(DaPostgreSql);
		OdbcManager::GetInstance()->DeleteAccessorObject(DaMySql);

		StkObject* DatObjMariaDb = new StkObject(_T("OdbcInfo"));
		DatObjMariaDb->AppendChildElement(new StkObject(_T("DbType"), _T("MariaDB")));
		DatObjMariaDb->AppendChildElement(new StkObject(_T("ConnStr"), ConnStrMariaDb));
		DatObj->AppendChildElement(DatObjMariaDb);

		StkObject* DatObjPostgreSql = new StkObject(_T("OdbcInfo"));
		DatObjPostgreSql->AppendChildElement(new StkObject(_T("DbType"), _T("PostgreSQL")));
		DatObjPostgreSql->AppendChildElement(new StkObject(_T("ConnStr"), ConnStrPostgreSql));
		DatObj->AppendChildElement(DatObjPostgreSql);

		StkObject* DatObjMySql = new StkObject(_T("OdbcInfo"));
		DatObjMySql->AppendChildElement(new StkObject(_T("DbType"), _T("MySQL")));
		DatObjMySql->AppendChildElement(new StkObject(_T("ConnStr"), ConnStrMySql));
		DatObj->AppendChildElement(DatObjMySql);
	}
	if (StrStr(UrlPath, _T("?query=configured"))) {
		TCHAR ConnStr[256];
		int Init;
		int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
		TCHAR DbmsTypeStr[16];
		if (Init == 1) {
			lstrcpy(DbmsTypeStr, _T("Init"));
		} else if (DbmsType == OdbcManager::MARIADB_ACCESSOR) {
			lstrcpy(DbmsTypeStr, _T("MariaDB"));
		} else if (DbmsType == OdbcManager::POSTGRESQL_ACCESSOR) {
			lstrcpy(DbmsTypeStr, _T("PostgreSQL"));
		} else if (DbmsType == OdbcManager::MYSQL_ACCESSOR) {
			lstrcpy(DbmsTypeStr, _T("MySQL"));
		} else {
		}
		StkObject* DatObjDb = new StkObject(_T("OdbcInfo"));
		DatObjDb->AppendChildElement(new StkObject(_T("DbType"), DbmsTypeStr));
		DatObjDb->AppendChildElement(new StkObject(_T("ConnStr"), ConnStr));
		DatObj->AppendChildElement(DatObjDb);
	}

	ResObj->AppendChildElement(DatObj);

	*ResultCode = 200;

	return ResObj;
}

void ApiOdbcInfos::PostOdbcInfos(StkObject*, int* ResultCode)
{
}

StkObject* ApiOdbcInfos::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	if (Method & STKWEBAPP_METHOD_GET) {
		return GetOdbcInfos(UrlPath, ResultCode);
	}
	return NULL;
}
