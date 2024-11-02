#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "MyMsgProc.h"
#include "ApiOdbcInfo.h"
#include "dataaccess.h"
#include "Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"

StkObject* ApiOdbcInfo::GetOdbcInfo(wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t* Token)
{
	StkObject* ResObj = new StkObject(L"");
	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	int YourId = -1;
	if (!CheckCredentials(Token, YourName, &YourId)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	if (StkPlWcsStr(UrlPath, L"?query=default")) {
		AddCodeAndMsg(ResObj, 0, L"", L"");
		StkObject* DatObj = new StkObject(L"Data");

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

		StkObject* DatObjMariaDb = new StkObject(L"OdbcInfo");
		DatObjMariaDb->AppendChildElement(new StkObject(L"DbType", L"MariaDB"));
		DatObjMariaDb->AppendChildElement(new StkObject(L"ConnStr", (wchar_t*)ConnStrMariaDb));
		DatObj->AppendChildElement(DatObjMariaDb);

		StkObject* DatObjPostgreSql = new StkObject(L"OdbcInfo");
		DatObjPostgreSql->AppendChildElement(new StkObject(L"DbType", L"PostgreSQL"));
		DatObjPostgreSql->AppendChildElement(new StkObject(L"ConnStr", (wchar_t*)ConnStrPostgreSql));
		DatObj->AppendChildElement(DatObjPostgreSql);

		StkObject* DatObjMySql = new StkObject(L"OdbcInfo");
		DatObjMySql->AppendChildElement(new StkObject(L"DbType", L"MySQL"));
		DatObjMySql->AppendChildElement(new StkObject(L"ConnStr", (wchar_t*)ConnStrMySql));
		DatObj->AppendChildElement(DatObjMySql);
		ResObj->AppendChildElement(DatObj);
		*ResultCode = 200;
	} else 
	if (StkPlWcsStr(UrlPath, L"?query=configured")) {
		AddCodeAndMsg(ResObj, 0, L"", L"");
		StkObject* DatObj = new StkObject(L"Data");

		wchar_t ConnStr[256];
		int Init;
		int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
		wchar_t DbmsTypeStr[16];
		if (Init == 1) {
			StkPlLStrCpy(DbmsTypeStr, L"Init");
		} else if (DbmsType == OdbcManager::MARIADB_ACCESSOR) {
			StkPlLStrCpy(DbmsTypeStr, L"MariaDB");
		} else if (DbmsType == OdbcManager::POSTGRESQL_ACCESSOR) {
			StkPlLStrCpy(DbmsTypeStr, L"PostgreSQL");
		} else if (DbmsType == OdbcManager::MYSQL_ACCESSOR) {
			StkPlLStrCpy(DbmsTypeStr, L"MySQL");
		} else {
		}

		StkObject* DatObjDb = new StkObject(L"OdbcInfo");
		DatObjDb->AppendChildElement(new StkObject(L"DbType", DbmsTypeStr));
		DatObjDb->AppendChildElement(new StkObject(L"ConnStr", ConnStr));

		DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
		int Ret = Da->Test((SQLTCHAR*)ConnStr);
		if (Ret == SQL_SUCCESS) {
			DatObjDb->AppendChildElement(new StkObject(L"Status", L"connectable"));
		} else {
			DatObjDb->AppendChildElement(new StkObject(L"Status", L"unconnectable"));
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

StkObject* ApiOdbcInfo::PostOdbcInfo(StkObject* ReqObj, int* ResultCode, wchar_t* Token)
{
	int DbmsType = -1;
	SQLTCHAR ConnStr[Global::MAX_PARAM_LENGTH];
	StkPlLStrCpy((wchar_t*)ConnStr, L"");

	StkObject* ResObj = new StkObject(L"");

	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	int YourId = -1;
	if (!CheckCredentials(Token, YourName, &YourId)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	if (!IsAdminUser(Token)) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_ACCESS_RIGHT_ERROR));
		*ResultCode = 403;
		return ResObj;
	}

	StkObject* Elem = ReqObj->GetFirstChildElement();
	while (Elem) {
		if (!StkPlWcsCmp(Elem->GetName(), L"DbType") && Elem->GetType() == StkObject::STKOBJECT_ELEM_STRING) {
			if (!StkPlWcsCmp(Elem->GetStringValue(), L"MariaDB")) {
				DbmsType = OdbcManager::MARIADB_ACCESSOR;
			} else
			if (!StkPlWcsCmp(Elem->GetStringValue(), L"PostgreSQL")) {
				DbmsType = OdbcManager::POSTGRESQL_ACCESSOR;
			} else
			if (!StkPlWcsCmp(Elem->GetStringValue(), L"MySQL")) {
				DbmsType = OdbcManager::MYSQL_ACCESSOR;
			}
		}
		if (!StkPlWcsCmp(Elem->GetName(), L"ConnStr") && Elem->GetType() == StkObject::STKOBJECT_ELEM_STRING) {
			size_t LenOfConnStr = StkPlWcsLen(Elem->GetStringValue());
			if (LenOfConnStr >= 256) {
				wchar_t TmpMsgEng[StkWebAppExec::URL_PATH_LENGTH];
				wchar_t TmpMsgJpn[StkWebAppExec::URL_PATH_LENGTH];
				StkPlSwPrintf(TmpMsgEng, StkWebAppExec::URL_PATH_LENGTH, L"%ls (Param=%ls, Length=%d, Max Length=255)", MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG), L"ConnStr", LenOfConnStr);
				StkPlSwPrintf(TmpMsgJpn, StkWebAppExec::URL_PATH_LENGTH, L"%ls (Param=%ls, Length=%d, Max Length=255)", MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG), L"ConnStr", LenOfConnStr);
				AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_PARAM_LENGTH_TOO_LONG, TmpMsgEng, TmpMsgJpn);
				*ResultCode = 400;
				return ResObj;
			}
			StkPlLStrCpy((wchar_t*)ConnStr, Elem->GetStringValue());
		}
		Elem = Elem->GetNext();
	}
	if (DbmsType == -1 || StkPlWcsCmp((wchar_t*)ConnStr, L"") == 0) {
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT),  MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_REQ_NOT_SUFFICIENT));
		*ResultCode = 400;
	} else {
		DataAccess::GetInstance()->SetOdbcConnStr(DbmsType, (wchar_t*)ConnStr);
		AddCodeAndMsg(ResObj, 0, L"", L"");
		StkObject* DatObj = new StkObject(L"Data");
		StkObject* DatObjDb = new StkObject(L"OdbcInfo");
		DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
		int Ret = Da->Test(ConnStr);
		if (Ret == SQL_SUCCESS) {
			DatObjDb->AppendChildElement(new StkObject(L"Status", L"connectable"));
		} else {
			DatObjDb->AppendChildElement(new StkObject(L"Status", L"unconnectable"));
		}
		OdbcManager::GetInstance()->DeleteAccessorObject(Da);
		DatObj->AppendChildElement(DatObjDb);
		ResObj->AppendChildElement(DatObj);

		StkWebAppUm_AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_LOG_DBMSCHANGE), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_LOG_DBMSCHANGE), YourId);
		*ResultCode = 200;
	}

	return ResObj;
}

StkObject* ApiOdbcInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	if (Method & STKWEBAPP_METHOD_GET) {
		return GetOdbcInfo(UrlPath, ResultCode, Token);
	} else
	if (Method & STKWEBAPP_METHOD_POST) {
		return PostOdbcInfo(ReqObj, ResultCode, Token);
	}
	return NULL;
}
