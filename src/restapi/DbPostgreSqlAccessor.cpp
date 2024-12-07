#ifdef WIN32
	#include <windows.h>
#endif
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "DbPostgreSqlAccessor.h"
#include "Global.h"
#include "dataaccess.h"

DbPostgreSqlAccessor::DbPostgreSqlAccessor()
{
}

DbPostgreSqlAccessor::~DbPostgreSqlAccessor()
{
}

void DbPostgreSqlAccessor::GetDefaultConnStr(wchar_t DefConnStr[Global::MAX_PARAM_LENGTH])
{
	StkPlLStrCpy(DefConnStr, L"Driver={PostgreSQL Unicode};Server=127.0.0.1;Database=DATABASE_NAME;UID=UID;PWD=PWD;Port=5432;");
}

int DbPostgreSqlAccessor::GetNumOfRecords(wchar_t* TableName, wchar_t StateMsg[10], wchar_t Msg[1024])
{
	size_t LenOfTableName = StkPlWcsLen((wchar_t*)TableName);
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
		SqlEncoding((SQLTCHAR*)ColumnName[Loop - 1], (SQLTCHAR*)ColumnNameCnv[Loop - 1], TYPE_KEY);
		if (FilterSwitch && (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11)) {
			SqlEncoding((SQLTCHAR*)Value[Loop - 1], (SQLTCHAR*)ValueCnv[Loop - 1], TYPE_LIKE_VALUE);
		} else {
			SqlEncoding((SQLTCHAR*)Value[Loop - 1], (SQLTCHAR*)ValueCnv[Loop - 1], TYPE_VALUE);
		}
	}

	int Ret = GetNumOfRecordsCommon(EcdTableName, ColumnNameCnv, OpeType, ValueCnv, StateMsg, Msg);
	delete EcdTableName;
	return Ret;
}

SQLRETURN DbPostgreSqlAccessor::GetTables(StkObject* Obj, wchar_t StateMsg[10], wchar_t Msg[1024])
{
	SQLRETURN Ret = 0;

	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}
	Ret = GetTablesCommon(L"select relname as TABLE_NAME from pg_stat_user_tables;", Obj, StateMsg, Msg);
	Ret = CloseDatabase(StateMsg, Msg);

	return Ret;
}

int DbPostgreSqlAccessor::GetColumnInfoByTableName(wchar_t* TableName, StkObject* TblObj, wchar_t StateMsg[10], wchar_t Msg[1024])
{
	SQLTCHAR CvtStateMsg[10];
	SQLTCHAR CvtMsg[1024];

	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg);
	if (Ret != SQL_SUCCESS) {
		return 0;
	}

	size_t LenOfTableName = StkPlWcsLen(TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_VALUE);

	wchar_t SqlBuf[1024];
	StkPlSwPrintf(SqlBuf, 1024, L"SELECT * FROM information_schema.columns WHERE table_schema='public' and table_name='%ls';", EcdTableName);
	char16_t* CvtSqlBuf = StkPlCreateUtf16FromWideChar(SqlBuf);
	Ret = SQLExecDirect(Hstmt, (SQLTCHAR*)CvtSqlBuf, SQL_NTS);
	delete CvtSqlBuf;
	delete EcdTableName;
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, CvtStateMsg, CvtMsg);
		return 0;
	}
	SQLTCHAR TmpColumnName[Global::COLUMNNAME_LENGTH];
	SQLTCHAR TmpColumnType[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR ColumnType[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR ColTypeCnv[Global::COLUMNTYPE_LENGTH];
	SQLTCHAR TmpIsNull[10];
	int TmpColumnMaxLen;
	SQLLEN ColumneNameLen, ColumneTypeLen, ColumneMaxLen, IsNullLen;
	SQLBindCol(Hstmt, 4, SQL_C_WCHAR, TmpColumnName, Global::COLUMNNAME_LENGTH * sizeof(SQLTCHAR), &ColumneNameLen);
	SQLBindCol(Hstmt, 7, SQL_C_WCHAR, TmpIsNull, 10 * sizeof(SQLTCHAR), &IsNullLen);
	SQLBindCol(Hstmt, 8, SQL_C_WCHAR, ColumnType, Global::COLUMNTYPE_LENGTH * sizeof(SQLTCHAR), &ColumneTypeLen);
	SQLBindCol(Hstmt, 9, SQL_C_SLONG, &TmpColumnMaxLen, 0, &ColumneMaxLen);

	int Loop = 0;
	for (; Loop < Global::MAXNUM_COLUMNS; Loop++) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO && ColumneMaxLen != SQL_NULL_DATA) {
			SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
			ConvertMessage(StateMsg, Msg, CvtStateMsg, CvtMsg);
			return 0;
		}
		if (ColumneMaxLen != SQL_NULL_DATA) {
			StkPlSwPrintf((wchar_t*)TmpColumnType, Global::COLUMNTYPE_LENGTH, L"%ls(%d)", ColumnType, TmpColumnMaxLen);
		} else {
			StkPlLStrCpy((wchar_t*)TmpColumnType, (wchar_t*)ColumnType);
		}
		ConvertAttrType(TmpColumnType, ColTypeCnv);
		StkObject* ClmObj = new StkObject(L"ColumnInfo");
		ClmObj->AppendChildElement(new StkObject(L"title", (wchar_t*)TmpColumnName));
		ClmObj->AppendChildElement(new StkObject(L"width", 100));
		ClmObj->AppendChildElement(new StkObject(L"dataType", (wchar_t*)ColTypeCnv));
		ClmObj->AppendChildElement(new StkObject(L"dataIndx", Loop));
		ClmObj->AppendChildElement(new StkObject(L"coltype", (wchar_t*)TmpColumnType));
		ClmObj->AppendChildElement(new StkObject(L"isnull", (wchar_t*)TmpIsNull));
		TblObj->AppendChildElement(ClmObj);
	}
	Ret = CloseDatabase(StateMsg, Msg);

	return Loop;
}

int DbPostgreSqlAccessor::GetRecordsByTableName(SQLTCHAR* TableName, int NumOfCols, StkObject* DatObj, SQLTCHAR* SortTarget, SQLTCHAR* SortOrder, int Limit, int Offset, wchar_t StateMsg[10], wchar_t Msg[1024])
{
	SQLRETURN Ret = 0;
	wchar_t ConnStr[256];
	int Init;

	size_t LenOfTableName = StkPlWcsLen((wchar_t*)TableName);
	SQLTCHAR* EcdTableName = new SQLTCHAR[LenOfTableName * 4 + 2];
	SqlEncoding(TableName, EcdTableName, TYPE_KEY);

	SQLTCHAR* EcdSortTarget = NULL;
	if (SortTarget != NULL && *SortTarget != L'\0') {
		size_t LenOfSortTarget = StkPlWcsLen((wchar_t*)SortTarget);
		EcdSortTarget = new  SQLTCHAR[LenOfSortTarget * 4 + 2];
		SqlEncoding(SortTarget, EcdSortTarget, TYPE_KEY);
	}

	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg);

	wchar_t ColumnName[5][Global::COLUMNNAME_LENGTH];
	wchar_t ColumnNameCnv[5][Global::COLUMNNAME_LENGTH * 4 + 2];
	int OpeType[5];
	wchar_t Value[5][Global::COLUMNVAL_LENGTH];
	wchar_t ValueCnv[5][Global::COLUMNVAL_LENGTH * 4 + 2];
	bool FilterSwitch = DataAccess::GetInstance()->GetFilterSwitch();
	for (int Loop = 1; Loop <= 5; Loop++) {
		DataAccess::GetInstance()->GetFilterCondition(Loop, ColumnName[Loop - 1], &OpeType[Loop - 1], Value[Loop - 1]);
		SqlEncoding((SQLTCHAR*)ColumnName[Loop - 1], (SQLTCHAR*)ColumnNameCnv[Loop - 1], TYPE_KEY);
		if (FilterSwitch && (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11)) {
			SqlEncoding((SQLTCHAR*)Value[Loop - 1], (SQLTCHAR*)ValueCnv[Loop - 1], TYPE_LIKE_VALUE);
		} else {
			SqlEncoding((SQLTCHAR*)Value[Loop - 1], (SQLTCHAR*)ValueCnv[Loop - 1], TYPE_VALUE);
		}
	}

	int NumOfRecs = GetRecordsByTableNameCommon(EcdTableName, NumOfCols, DatObj, ColumnNameCnv, OpeType, ValueCnv, (wchar_t*)EcdSortTarget, (wchar_t*)SortOrder, Limit, Offset, StateMsg, Msg);

	delete EcdTableName;
	if (EcdSortTarget != NULL) {
		delete EcdSortTarget;
	}
	Ret = CloseDatabase(StateMsg, Msg);
	return NumOfRecs;
}

int DbPostgreSqlAccessor::ConvertAttrType(SQLTCHAR InAttr[Global::COLUMNTYPE_LENGTH], SQLTCHAR OutAttr[Global::COLUMNTYPE_LENGTH])
{
	if (StkPlWcsStr((wchar_t*)InAttr, L"bigint") != NULL ||
		StkPlWcsStr((wchar_t*)InAttr, L"integer") != NULL ||
		StkPlWcsStr((wchar_t*)InAttr, L"smallint") != NULL ||
		StkPlWcsStr((wchar_t*)InAttr, L"bigserial") != NULL ||
		StkPlWcsStr((wchar_t*)InAttr, L"serial") != NULL ||
		StkPlWcsStr((wchar_t*)InAttr, L"smallserial") != NULL) {
		StkPlLStrCpy((wchar_t*)OutAttr, L"integer");
		return 0;
	}
	if (StkPlWcsStr((wchar_t*)InAttr, L"real") != NULL ||
		StkPlWcsStr((wchar_t*)InAttr, L"double precision") != NULL) {
		StkPlLStrCpy((wchar_t*)OutAttr, L"float");
		return 0;
	}
	StkPlLStrCpy((wchar_t*)OutAttr, L"string");
	return 0;
}

void DbPostgreSqlAccessor::SqlEncoding(wchar_t* InSql, wchar_t* OutSql, int Type)
{
	size_t LenOfInSql = StkPlWcsLen(InSql);
	int OutSqlIndex = 0;
	if (Type == TYPE_KEY) {
		OutSql[0] = L'\"';
		OutSqlIndex++;
	}
	for (int Loop = 0; Loop < LenOfInSql; Loop++) {
		if (InSql[Loop] == L'\"' && Type == TYPE_KEY) {
			StkPlLStrCpy(&OutSql[OutSqlIndex], L"\"\"");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'\'' && (Type == TYPE_VALUE || Type == TYPE_LIKE_VALUE)) {
			StkPlLStrCpy(&OutSql[OutSqlIndex], L"\'\'");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'\\' && Type == TYPE_LIKE_VALUE) {
			StkPlLStrCpy(&OutSql[OutSqlIndex], L"\\\\");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'%' && Type == TYPE_LIKE_VALUE) {
			StkPlLStrCpy(&OutSql[OutSqlIndex], L"\\%");
			OutSqlIndex += 2;
			continue;
		}
		if (InSql[Loop] == L'_' && Type == TYPE_LIKE_VALUE) {
			StkPlLStrCpy(&OutSql[OutSqlIndex], L"\\_");
			OutSqlIndex += 2;
			continue;
		}
		OutSql[OutSqlIndex] = InSql[Loop];
		OutSqlIndex++;
	}
	if (Type == TYPE_KEY) {
		OutSql[OutSqlIndex] = L'\"';
		OutSqlIndex++;
	}
	OutSql[OutSqlIndex] = L'\0';
}
