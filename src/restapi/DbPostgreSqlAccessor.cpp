#include <windows.h>
#include "DbPostgreSqlAccessor.h"
#include "..\Global.h"
#include "dataaccess.h"

DbPostgreSqlAccessor::DbPostgreSqlAccessor()
{
}

DbPostgreSqlAccessor::~DbPostgreSqlAccessor()
{
}

void DbPostgreSqlAccessor::GetDefaultConnStr(SQLTCHAR DefConnStr[Global::MAX_PARAM_LENGTH])
{
	lstrcpy(DefConnStr, _T("Driver={PostgreSQL Unicode};Server=127.0.0.1;Database=DATABASE_NAME;UID=UID;PWD=PWD;Port=5432;"));
}

int DbPostgreSqlAccessor::GetNumOfRecords(SQLTCHAR* TableName, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	int LenOfTableName = lstrlen((TCHAR*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	int Ret = GetNumOfRecordsCommon(EcdTableName, StateMsg, Msg, MsgLen);
	delete EcdTableName;
	return Ret;
}

SQLRETURN DbPostgreSqlAccessor::GetTables(StkObject* Obj, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLRETURN Ret = 0;

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}
	Ret = GetTablesCommon(_T("select relname as TABLE_NAME from pg_stat_user_tables;"), Obj, StateMsg, Msg, MsgLen);
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);

	return Ret;
}

int DbPostgreSqlAccessor::GetColumnInfoByTableName(SQLTCHAR* TableName,
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
	SqlEncoding(TableName, EcdTableName, TYPE_VALUE);

	SQLTCHAR SqlBuf[1024];
	_snwprintf_s(SqlBuf, 1024, _TRUNCATE, _T("SELECT * FROM information_schema.columns WHERE table_schema='public' and table_name='%s';"), EcdTableName);
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	delete EcdTableName;
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return 0;
	}
	SQLTCHAR TmpColumneName[Global::COLUMNNAME_LENGTH];
	SQLTCHAR TmpColumneType[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR TmpIsNull[10];
	int TmpColumnMaxLen;
	SQLINTEGER ColumneNameLen, ColumneTypeLen, ColumneMaxLen, IsNullLen;
	SQLBindCol(Hstmt, 4, SQL_C_WCHAR, TmpColumneName, Global::COLUMNNAME_LENGTH * sizeof(SQLTCHAR), &ColumneNameLen);
	SQLBindCol(Hstmt, 7, SQL_C_WCHAR, TmpIsNull, 10 * sizeof(SQLTCHAR), &IsNullLen);
	SQLBindCol(Hstmt, 8, SQL_C_WCHAR, TmpColumneType, Global::COLUMNTYPE_LENGTH * sizeof(SQLTCHAR), &ColumneTypeLen);
	SQLBindCol(Hstmt, 9, SQL_C_SLONG, &TmpColumnMaxLen, 0, &ColumneMaxLen);

	int Loop = 0;
	for (; Loop < Global::MAXNUM_COLUMNS; Loop++) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO && ColumneMaxLen != SQL_NULL_DATA) {
			SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return 0;
		}
		lstrcpy(ColumnName[Loop], TmpColumneName);
		if (ColumneMaxLen != SQL_NULL_DATA) {
			_snwprintf_s(ColumnType[Loop], Global::COLUMNTYPE_LENGTH, _TRUNCATE, _T("%s(%d)"), TmpColumneType, TmpColumnMaxLen);
		} else {
			lstrcpy(ColumnType[Loop], TmpColumneType);
		}
		lstrcpy(IsNull[Loop], TmpIsNull);
	}
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);

	return Loop;
}

int DbPostgreSqlAccessor::GetRecordsByTableName(SQLTCHAR* TableName,
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

int DbPostgreSqlAccessor::ConvertAttrType(SQLTCHAR InAttr[Global::COLUMNTYPE_LENGTH], SQLTCHAR OutAttr[Global::COLUMNTYPE_LENGTH])
{
	if (wcsstr(InAttr, _T("bigint")) != NULL ||
		wcsstr(InAttr, _T("integer")) != NULL ||
		wcsstr(InAttr, _T("smallint")) != NULL ||
		wcsstr(InAttr, _T("bigserial")) != NULL ||
		wcsstr(InAttr, _T("serial")) != NULL ||
		wcsstr(InAttr, _T("smallserial")) != NULL) {
		lstrcpy(OutAttr, _T("integer"));
		return 0;
	}
	if (wcsstr(InAttr, _T("real")) != NULL ||
		wcsstr(InAttr, _T("double precision")) != NULL) {
		lstrcpy(OutAttr, _T("float"));
		return 0;
	}
	lstrcpy(OutAttr, _T("string"));
	return 0;
}

void DbPostgreSqlAccessor::SqlEncoding(SQLTCHAR* InSql, SQLTCHAR* OutSql, int Type)
{
	int LenOfInSql = lstrlen(InSql);
	int OutSqlIndex = 0;
	if (Type == TYPE_KEY) {
		OutSql[0] = _T('\"');
		OutSqlIndex++;
	}
	for (int Loop = 0; Loop < LenOfInSql; Loop++) {
		if (InSql[Loop] == _T('\"') && Type == TYPE_KEY) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\"\""));
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == _T('\'') && (Type == TYPE_VALUE || Type == TYPE_LIKE_VALUE)) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\'\'"));
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == _T('\\') && Type == TYPE_LIKE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], _T("\\\\"));
			OutSqlIndex += 2;
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
		OutSql[OutSqlIndex] = _T('\"');
		OutSqlIndex++;
	}
	OutSql[OutSqlIndex] = _T('\0');
}
