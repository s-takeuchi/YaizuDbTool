#include <windows.h>
#include "DbMySqlAccessor.h"
#include "..\Global.h"
#include "dataaccess.h"

DbMySqlAccessor::DbMySqlAccessor()
{
}

DbMySqlAccessor::~DbMySqlAccessor()
{
}

void DbMySqlAccessor::GetDefaultConnStr(SQLTCHAR DefConnStr[Global::MAX_PARAM_LENGTH])
{
	lstrcpy(DefConnStr, _T("Driver={MySQL ODBC 5.2 Unicode Driver};Server=localhost;Port=3306;Option=131072;Stmt=;Database=DATABASE_NAME;Uid=UID;Pwd=PWD;"));
}

int DbMySqlAccessor::GetNumOfRecords(SQLTCHAR* TableName, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	int LenOfTableName = lstrlen((TCHAR*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	int Ret = GetNumOfRecordsCommon(EcdTableName, StateMsg, Msg, MsgLen);
	delete EcdTableName;
	return Ret;
}

SQLRETURN DbMySqlAccessor::GetTables(StkObject* Obj, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLRETURN Ret = 0;

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}
	Ret = GetTablesCommon(_T("show tables;"), Obj, StateMsg, Msg, MsgLen);
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);

	return Ret;
}

int DbMySqlAccessor::GetColumnInfoByTableName(SQLTCHAR* TableName, StkObject* Obj, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);
	if (Ret != SQL_SUCCESS) {
		return 0;
	}

	int LenOfTableName = lstrlen((TCHAR*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	SQLTCHAR SqlBuf[1024];
	_snwprintf_s(SqlBuf, 1024, _TRUNCATE, _T("show full columns from %s;"), EcdTableName);
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	delete EcdTableName;
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return 0;
	}
	SQLTCHAR TmpColumneName[Global::COLUMNNAME_LENGTH];
	SQLTCHAR TmpColumneType[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR TmpIsNull[10];
	SQLBindCol(Hstmt, 1, SQL_C_WCHAR, TmpColumneName, Global::COLUMNNAME_LENGTH * sizeof(SQLTCHAR), NULL);
	SQLBindCol(Hstmt, 2, SQL_C_WCHAR, TmpColumneType, Global::COLUMNTYPE_LENGTH * sizeof(SQLTCHAR), NULL);
	SQLBindCol(Hstmt, 4, SQL_C_WCHAR, TmpIsNull, 10 * sizeof(SQLTCHAR), NULL);

	int Loop = 0;
	for (; Loop < Global::MAXNUM_COLUMNS; Loop++) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO) {
			SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return 0;
		}
		//lstrcpy(ColumnName[Loop], TmpColumneName);
		//lstrcpy(ColumnType[Loop], TmpColumneType);
		//lstrcpy(IsNull[Loop], TmpIsNull);
	}
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);

	return Loop;
}

int DbMySqlAccessor::GetColumnInfoByTableName(SQLTCHAR* TableName,
	SQLTCHAR ColumnName[Global::MAXNUM_COLUMNS][Global::COLUMNNAME_LENGTH],
	SQLTCHAR ColumnType[Global::MAXNUM_COLUMNS][Global::COLUMNTYPE_LENGTH],
	SQLTCHAR IsNull[Global::MAXNUM_COLUMNS][10],
	SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);
	if (Ret != SQL_SUCCESS) {
		return 0;
	}

	int LenOfTableName = lstrlen((TCHAR*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	SQLTCHAR SqlBuf[1024];
	_snwprintf_s(SqlBuf, 1024, _TRUNCATE, _T("show full columns from %s;"), EcdTableName);
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	delete EcdTableName;
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return 0;
	}
	SQLTCHAR TmpColumneName[Global::COLUMNNAME_LENGTH];
	SQLTCHAR TmpColumneType[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR TmpIsNull[10];
	SQLBindCol(Hstmt, 1, SQL_C_WCHAR, TmpColumneName, Global::COLUMNNAME_LENGTH * sizeof(SQLTCHAR), NULL);
	SQLBindCol(Hstmt, 2, SQL_C_WCHAR, TmpColumneType, Global::COLUMNTYPE_LENGTH * sizeof(SQLTCHAR), NULL);
	SQLBindCol(Hstmt, 4, SQL_C_WCHAR, TmpIsNull, 10 * sizeof(SQLTCHAR), NULL);

	int Loop = 0;
	for (; Loop < Global::MAXNUM_COLUMNS; Loop++) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO) {
			SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return 0;
		}
		lstrcpy(ColumnName[Loop], TmpColumneName);
		lstrcpy(ColumnType[Loop], TmpColumneType);
		lstrcpy(IsNull[Loop], TmpIsNull);
	}
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);

	return Loop;
}

int DbMySqlAccessor::GetRecordsByTableName(SQLTCHAR* TableName,
	int NumOfCols,
	SQLTCHAR* Record, int NumOfRecords,
	SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLRETURN Ret = 0;
	TCHAR ConnStr[256];
	int Init;
	int LenOfTableName = lstrlen((TCHAR*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);

	TCHAR ColumnName[5][Global::COLUMNNAME_LENGTH];
	TCHAR ColumnNameCnv[5][Global::COLUMNNAME_LENGTH * 4 + 2];
	int OpeType[5];
	TCHAR Value[5][Global::COLUMNVAL_LENGTH];
	TCHAR ValueCnv[5][Global::COLUMNVAL_LENGTH * 4 + 2];
	BOOL FilterSwitch = DataAccess::GetInstance()->GetFilterSwitch();
	for (int Loop = 1; Loop <= 5; Loop++) {
		DataAccess::GetInstance()->GetFilterCondition(Loop, ColumnName[Loop - 1], &OpeType[Loop - 1], Value[Loop - 1]);
		SqlEncoding(ColumnName[Loop - 1], ColumnNameCnv[Loop - 1], TYPE_KEY);
		if (FilterSwitch && (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11)) {
			SqlEncoding(Value[Loop - 1], ValueCnv[Loop - 1], TYPE_LIKE_VALUE);
		} else {
			SqlEncoding(Value[Loop - 1], ValueCnv[Loop - 1], TYPE_VALUE);
		}
	}

	int NumOfRecs = GetRecordsByTableNameCommon(EcdTableName, NumOfCols, Record, NumOfRecords, ColumnNameCnv, OpeType, ValueCnv, StateMsg, Msg, MsgLen);
	delete EcdTableName;
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);
	return NumOfRecs;
}

int DbMySqlAccessor::ConvertAttrType(SQLTCHAR InAttr[Global::COLUMNTYPE_LENGTH], SQLTCHAR OutAttr[Global::COLUMNTYPE_LENGTH])
{
	if (wcsstr(InAttr, _T("bigint")) != NULL ||
		wcsstr(InAttr, _T("int")) != NULL ||
		wcsstr(InAttr, _T("mediumint")) != NULL ||
		wcsstr(InAttr, _T("smallint")) != NULL ||
		wcsstr(InAttr, _T("tinyint")) != NULL) {
		lstrcpy(OutAttr, _T("integer"));
		return 0;
	}
	if (wcsstr(InAttr, _T("float")) != NULL ||
		wcsstr(InAttr, _T("double")) != NULL ||
		wcsstr(InAttr, _T("double precision")) != NULL) {
		lstrcpy(OutAttr, _T("float"));
		return 0;
	}
	lstrcpy(OutAttr, _T("string"));
	return 0;
}

void DbMySqlAccessor::SqlEncoding(SQLTCHAR* InSql, SQLTCHAR* OutSql, int Type)
{
	int LenOfInSql = lstrlen(InSql);
	int OutSqlIndex = 0;
	if (Type == TYPE_KEY) {
		OutSql[0] = _T('`');
		OutSqlIndex++;
	}
	for (int Loop = 0; Loop < LenOfInSql; Loop++) {
		if (InSql[Loop] == _T('`') && Type == TYPE_KEY) {
			lstrcpy(&OutSql[OutSqlIndex], _T("``"));
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == _T('\'') && (Type == TYPE_VALUE || Type == TYPE_LIKE_VALUE)) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\'\'"));
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == _T('\\') && Type == TYPE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\\\\"));
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == _T('\\') && Type == TYPE_LIKE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\\\\\\\\"));
			OutSqlIndex += 4;
			continue;
		}
		if (InSql[Loop] == _T('%') && Type == TYPE_LIKE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\\%"));
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == _T('_') && Type == TYPE_LIKE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\\_"));
			OutSqlIndex += 2;
			continue;
		}
		OutSql[OutSqlIndex] = InSql[Loop];
		OutSqlIndex++;
	}
	if (Type == TYPE_KEY) {
		OutSql[OutSqlIndex] = _T('`');
		OutSqlIndex++;
	}
	OutSql[OutSqlIndex] = _T('\0');
}
