#include <windows.h>
#include "DbMariaDbAccessor.h"
#include "..\Global.h"
#include "dataaccess.h"

DbMariaDbAccessor::DbMariaDbAccessor()
{
}

DbMariaDbAccessor::~DbMariaDbAccessor()
{
}

void DbMariaDbAccessor::GetDefaultConnStr(SQLTCHAR DefConnStr[Global::MAX_PARAM_LENGTH])
{
	lstrcpy(DefConnStr, L"Driver={MariaDB ODBC 2.0 Driver};Server=localhost;UID=UID;PWD=PWD;DB=DATABASE_NAME;Port=3306;");
}

int DbMariaDbAccessor::GetNumOfRecords(SQLTCHAR* TableName, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	int LenOfTableName = lstrlen((wchar_t*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	wchar_t ColumnName[5][Global::COLUMNNAME_LENGTH];
	wchar_t ColumnNameCnv[5][Global::COLUMNNAME_LENGTH * 4 + 2];
	int OpeType[5];
	wchar_t Value[5][Global::COLUMNVAL_LENGTH];
	wchar_t ValueCnv[5][Global::COLUMNVAL_LENGTH * 4 + 2];
	bool FilterSwitch = DataAccess::GetInstance()->GetFilterSwitch();
	for (int Loop = 1; Loop <= 5; Loop++) {
		DataAccess::GetInstance()->GetFilterCondition(Loop, ColumnName[Loop - 1], &OpeType[Loop - 1], Value[Loop - 1]);
		SqlEncoding(ColumnName[Loop - 1], ColumnNameCnv[Loop - 1], TYPE_KEY);
		if (FilterSwitch && (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11)) {
			SqlEncoding(Value[Loop - 1], ValueCnv[Loop - 1], TYPE_LIKE_VALUE);
		} else {
			SqlEncoding(Value[Loop - 1], ValueCnv[Loop - 1], TYPE_VALUE);
		}
	}

	int Ret = GetNumOfRecordsCommon(EcdTableName, ColumnNameCnv, OpeType, ValueCnv, StateMsg, Msg, MsgLen);
	delete EcdTableName;
	return Ret;
}

SQLRETURN DbMariaDbAccessor::GetTables(StkObject* Obj, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLRETURN Ret = 0;

	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}
	Ret = GetTablesCommon(L"show tables;", Obj, StateMsg, Msg, MsgLen);
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);

	return Ret;
}

int DbMariaDbAccessor::GetColumnInfoByTableName(SQLTCHAR* TableName, StkObject* TblObj, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);
	if (Ret != SQL_SUCCESS) {
		return 0;
	}

	int LenOfTableName = lstrlen((wchar_t*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	SQLTCHAR SqlBuf[1024];
	StkPlSwPrintf(SqlBuf, 1024, L"show full columns from %ls;", EcdTableName);
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	delete EcdTableName;
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return 0;
	}
	SQLTCHAR TmpColumnNameTmp[Global::COLUMNNAME_LENGTH * 2]; // For adaptation to the bug of MariaDB ODBC connector
	SQLTCHAR TmpColumnTypeTmp[Global::COLUMNTYPE_LENGTH * 2]; // For adaptation to the bug of MariaDB ODBC connector
	SQLTCHAR TmpColumnName[Global::COLUMNNAME_LENGTH];
	SQLTCHAR TmpColumnType[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR ColTypeCnv[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR TmpIsNull[10];
	SQLINTEGER ColumneNameLen = 0;
	SQLINTEGER ColumneTypeLen = 0;
	SQLBindCol(Hstmt, 1, SQL_C_WCHAR, TmpColumnNameTmp, Global::COLUMNNAME_LENGTH * sizeof(SQLTCHAR), &ColumneNameLen);
	SQLBindCol(Hstmt, 2, SQL_C_WCHAR, TmpColumnTypeTmp, Global::COLUMNTYPE_LENGTH * sizeof(SQLTCHAR), &ColumneTypeLen);
	SQLBindCol(Hstmt, 4, SQL_C_WCHAR, TmpIsNull, 10 * sizeof(SQLTCHAR), NULL);

	int Loop = 0;
	for (; Loop < Global::MAXNUM_COLUMNS; Loop++) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO) {
			SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return 0;
		}
		lstrcpyn(TmpColumnName, TmpColumnNameTmp, Global::COLUMNNAME_LENGTH); // For adaptation to the bug of MariaDB ODBC connector
		lstrcpyn(TmpColumnType, TmpColumnTypeTmp, Global::COLUMNTYPE_LENGTH); // For adaptation to the bug of MariaDB ODBC connector
		ConvertAttrType(TmpColumnType, ColTypeCnv);
		StkObject* ClmObj = new StkObject(L"ColumnInfo");
		ClmObj->AppendChildElement(new StkObject(L"title", TmpColumnName));
		ClmObj->AppendChildElement(new StkObject(L"width", 100));
		ClmObj->AppendChildElement(new StkObject(L"dataType", ColTypeCnv));
		ClmObj->AppendChildElement(new StkObject(L"dataIndx", Loop));
		ClmObj->AppendChildElement(new StkObject(L"coltype", TmpColumnType));
		ClmObj->AppendChildElement(new StkObject(L"isnull", TmpIsNull));
		TblObj->AppendChildElement(ClmObj);
	}
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);

	return Loop;
}

int DbMariaDbAccessor::GetRecordsByTableName(SQLTCHAR* TableName, int NumOfCols, StkObject* DatObj, SQLTCHAR* SortTarget, SQLTCHAR* SortOrder, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLRETURN Ret = 0;
	wchar_t ConnStr[256];
	int Init;

	int LenOfTableName = lstrlen((wchar_t*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	SQLTCHAR* EcdSortTarget = NULL;
	if (SortTarget != NULL && *SortTarget != L'\0') {
		int LenOfSortTarget = lstrlen((wchar_t*)SortTarget);
		EcdSortTarget = new  SQLTCHAR[LenOfSortTarget * 4 + 2];
		SqlEncoding(SortTarget, EcdSortTarget, TYPE_KEY);
	}

	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, MsgLen);

	wchar_t ColumnName[5][Global::COLUMNNAME_LENGTH];
	wchar_t ColumnNameCnv[5][Global::COLUMNNAME_LENGTH * 4 + 2];
	int OpeType[5];
	wchar_t Value[5][Global::COLUMNVAL_LENGTH];
	wchar_t ValueCnv[5][Global::COLUMNVAL_LENGTH * 4 + 2];
	bool FilterSwitch = DataAccess::GetInstance()->GetFilterSwitch();
	for (int Loop = 1; Loop <= 5; Loop++) {
		DataAccess::GetInstance()->GetFilterCondition(Loop, ColumnName[Loop - 1], &OpeType[Loop - 1], Value[Loop - 1]);
		SqlEncoding(ColumnName[Loop - 1], ColumnNameCnv[Loop - 1], TYPE_KEY);
		if (FilterSwitch && (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11)) {
			SqlEncoding(Value[Loop - 1], ValueCnv[Loop - 1], TYPE_LIKE_VALUE);
		} else {
			SqlEncoding(Value[Loop - 1], ValueCnv[Loop - 1], TYPE_VALUE);
		}
	}

	int NumOfRecs = GetRecordsByTableNameCommon(EcdTableName, NumOfCols, DatObj, ColumnNameCnv, OpeType, ValueCnv, NULL, NULL, StateMsg, Msg, MsgLen);

	delete EcdTableName;
	if (EcdSortTarget != NULL) {
		delete EcdSortTarget;
	}
	Ret = CloseDatabase(StateMsg, Msg, MsgLen);
	return NumOfRecs;
}

int DbMariaDbAccessor::ConvertAttrType(SQLTCHAR InAttr[Global::COLUMNTYPE_LENGTH], SQLTCHAR OutAttr[Global::COLUMNTYPE_LENGTH])
{
	if (wcsstr(InAttr, L"bigint") != NULL ||
		wcsstr(InAttr, L"int") != NULL ||
		wcsstr(InAttr, L"mediumint") != NULL ||
		wcsstr(InAttr, L"smallint") != NULL ||
		wcsstr(InAttr, L"tinyint") != NULL) {
		lstrcpy(OutAttr, L"integer");
		return 0;
	}
	if (wcsstr(InAttr, L"float") != NULL ||
		wcsstr(InAttr, L"double") != NULL ||
		wcsstr(InAttr, L"double precision") != NULL) {
		lstrcpy(OutAttr, L"float");
		return 0;
	}
	lstrcpy(OutAttr, L"string");
	return 0;
}

void DbMariaDbAccessor::SqlEncoding(SQLTCHAR* InSql, SQLTCHAR* OutSql, int Type)
{
	int LenOfInSql = lstrlen(InSql);
	int OutSqlIndex = 0;
	if (Type == TYPE_KEY) {
		OutSql[0] = L'`';
		OutSqlIndex++;
	}
	for (int Loop = 0; Loop < LenOfInSql; Loop++) {
		if (InSql[Loop] == L'`' && Type == TYPE_KEY) {
			lstrcpy(&OutSql[OutSqlIndex], L"``");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'\'' && (Type == TYPE_VALUE || Type == TYPE_LIKE_VALUE)) {
			lstrcpy(&OutSql[OutSqlIndex], L"\'\'");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'\\' && Type == TYPE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], L"\\\\");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'\\' && Type == TYPE_LIKE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], L"\\\\\\\\");
			OutSqlIndex += 4;
			continue;
		}
		if (InSql[Loop] == L'%' && Type == TYPE_LIKE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], L"\\%");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'_' && Type == TYPE_LIKE_VALUE) {
			lstrcpy(&OutSql[OutSqlIndex], L"\\_");
			OutSqlIndex += 2;
			continue;
		}
		OutSql[OutSqlIndex] = InSql[Loop];
		OutSqlIndex++;
	}
	if (Type == TYPE_KEY) {
		OutSql[OutSqlIndex] = L'`';
		OutSqlIndex++;
	}
	OutSql[OutSqlIndex] = L'\0';
}
