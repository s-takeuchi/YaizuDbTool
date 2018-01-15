#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "MyMsgProc.h"
#include "ApiOdbcInfo.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"

StkObject* ApiOdbcInfo::GetOdbcInfo(TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode)
{
	StkObject* ResObj = new StkObject(_T(""));

	if (StrStr(UrlPath, _T("?query=default"))) {
		AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
		StkObject* DatObj = new StkObject(_T("Data"));

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
		ResObj->AppendChildElement(DatObj);
		*ResultCode = 200;
	} else 
	if (StrStr(UrlPath, _T("?query=configured"))) {
		AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
		StkObject* DatObj = new StkObject(_T("Data"));

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

		DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
		int Ret = Da->Test(ConnStr);
		if (Ret == SQL_SUCCESS) {
			DatObjDb->AppendChildElement(new StkObject(_T("Status"), _T("connectable")));
		} else {
			DatObjDb->AppendChildElement(new StkObject(_T("Status"), _T("unconnectable")));
		}
		OdbcManager::GetInstance()->DeleteAccessorObject(Da);

		DatObj->AppendChildElement(DatObjDb);
		ResObj->AppendChildElement(DatObj);
		*ResultCode = 200;
	} else {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
	}

	return ResObj;
}

StkObject* ApiOdbcInfo::PostOdbcInfo(StkObject* ReqObj, int* ResultCode)
{
	int DbmsType = -1;
	SQLTCHAR ConnStr[Global::MAX_PARAM_LENGTH] = _T("");

	StkObject* ResObj = new StkObject(_T(""));

	StkObject* Elem = ReqObj->GetFirstChildElement();
	while (Elem) {
		if (!lstrcmp(Elem->GetName(), _T("DbType")) && Elem->GetType() == StkObject::STKOBJECT_ELEM_STRING) {
			if (!lstrcmp(Elem->GetStringValue(), _T("MariaDB"))) {
				DbmsType = OdbcManager::MARIADB_ACCESSOR;
			} else
			if (!lstrcmp(Elem->GetStringValue(), _T("PostgreSQL"))) {
				DbmsType = OdbcManager::POSTGRESQL_ACCESSOR;
			} else
			if (!lstrcmp(Elem->GetStringValue(), _T("MySQL"))) {
				DbmsType = OdbcManager::MYSQL_ACCESSOR;
			}
		}
		if (!lstrcmp(Elem->GetName(), _T("ConnStr")) && Elem->GetType() == StkObject::STKOBJECT_ELEM_STRING) {
			int LenOfConnStr = lstrlen(Elem->GetStringValue());
			if (LenOfConnStr >= 256) {
				TCHAR TmpMsgEng[StkWebAppExec::URL_PATH_LENGTH];
				TCHAR TmpMsgJpn[StkWebAppExec::URL_PATH_LENGTH];
				swprintf_s(TmpMsgEng, StkWebAppExec::URL_PATH_LENGTH, _T("%s (Param=%s, Length=%d, Max Length=255)"), MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG), _T("ConnStr"), LenOfConnStr);
				swprintf_s(TmpMsgJpn, StkWebAppExec::URL_PATH_LENGTH, _T("%s (Param=%s, Length=%d, Max Length=255)"), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG), _T("ConnStr"), LenOfConnStr);
				AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG, TmpMsgEng, TmpMsgJpn);
				*ResultCode = 400;
				return ResObj;
			}
			lstrcpy(ConnStr, Elem->GetStringValue());
		}
		Elem = Elem->GetNext();
	}
	if (DbmsType == -1 || lstrcmp(ConnStr, _T("")) == 0) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
	} else {
		DataAccess::GetInstance()->SetOdbcConnStr(DbmsType, (TCHAR*)ConnStr);
		AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
		StkObject* DatObj = new StkObject(_T("Data"));
		StkObject* DatObjDb = new StkObject(_T("OdbcInfo"));
		DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
		int Ret = Da->Test(ConnStr);
		if (Ret == SQL_SUCCESS) {
			DatObjDb->AppendChildElement(new StkObject(_T("Status"), _T("connectable")));
		} else {
			DatObjDb->AppendChildElement(new StkObject(_T("Status"), _T("unconnectable")));
		}
		OdbcManager::GetInstance()->DeleteAccessorObject(Da);
		DatObj->AppendChildElement(DatObjDb);
		ResObj->AppendChildElement(DatObj);

		DataAccess::GetInstance()->AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_LOG_DBMSCHANGE), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_LOG_DBMSCHANGE));
		*ResultCode = 200;
	}

	return ResObj;
}

StkObject* ApiOdbcInfo::ExecuteImpl(StkObject* ReqObj, int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, TCHAR Locale[3])
{
	if (Method & STKWEBAPP_METHOD_GET) {
		return GetOdbcInfo(UrlPath, ResultCode);
	} else
	if (Method & STKWEBAPP_METHOD_POST) {
		return PostOdbcInfo(ReqObj, ResultCode);
	}
	return NULL;
}
