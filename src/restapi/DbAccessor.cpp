#ifdef WIN32
    #include <windows.h>
#endif
#include "DbAccessor.h"
#include "Global.h"
#include "dataaccess.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"

DbAccessor::DbAccessor()
{
}

DbAccessor::~DbAccessor()
{
}

int DbAccessor::Test(SQLTCHAR ConnStr[Global::MAX_PARAM_LENGTH], wchar_t ErrMsg[1024])
{
	char LogBuf[1024] = "";
	bool UnicodeDef = false;
	bool SqlwchartconvertDef = false;

#ifdef UNICODE
	UnicodeDef = true;
#endif
#ifdef SQL_WCHART_CONVERT
	SqlwchartconvertDef = true;
#endif
#ifndef WIN32
	StkPlSPrintf(LogBuf, 1024, "unixODBC definition [UNICODE=%s, SQL_WCHART_CONVER=%s]",
		(UnicodeDef == true)? "true" : "false",
		(SqlwchartconvertDef == true)? "true" : "false"
	);
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

	StkPlSPrintf(LogBuf, 1024, "Size of SQLTCHAR=%d", sizeof(SQLTCHAR));
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);
#endif

	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];
	int Ret = 0;
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, 1024);
	if (Ret != SQL_SUCCESS) {
		StkPlSwPrintf(ErrMsg, 1024, L"%ls : %ls", (StateMsg == NULL)? L"null" : (wchar_t*)StateMsg, (Msg == NULL)? L"null" : (wchar_t*)Msg);
		return Ret;
	}
	Ret = CloseDatabase(StateMsg, Msg, 1024);
	if (Ret != SQL_SUCCESS) {
		StkPlSwPrintf(ErrMsg, 1024, L"%ls : %ls", (StateMsg == NULL)? L"null" : (wchar_t*)StateMsg, (Msg == NULL)? L"null" : (wchar_t*)Msg);
		return Ret;
	}
	StkPlWcsCpy(ErrMsg, 1024, L"");
	return SQL_SUCCESS;
}

SQLRETURN DbAccessor::GetTablesCommon(SQLTCHAR* Query, StkObject* Obj, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	// SQLExecDirect
	Ret = SQLExecDirect(Hstmt, Query, SQL_NTS);
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRecW(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return Ret;
	}
	SQLTCHAR TableName[Global::TABLENAME_LENGTH];
	SQLBindCol(Hstmt, 1, SQL_C_WCHAR, TableName, Global::TABLENAME_LENGTH * sizeof(SQLTCHAR), NULL);

	bool InitFlag = true;
	int Loop = 0;
	for (;;) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO) {
			SQLGetDiagRecW(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return Ret;
		}
		StkObject* TblInfObj = new StkObject(L"TableInfo");
		TblInfObj->AppendChildElement(new StkObject(L"Name", (wchar_t*)TableName));
		Obj->AppendChildElement(TblInfObj);
		Loop++;
		if (Loop >= Global::MAXNUM_TABLES) {
			break;
		}
	}

	return Ret;
}

int DbAccessor::GetNumOfRecordsCommon(SQLTCHAR* TableName, wchar_t ColumnNameCnv[5][Global::COLUMNNAME_LENGTH * 4 + 2], int OpeType[5], wchar_t Value[5][Global::COLUMNVAL_LENGTH * 4 + 2], SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase((SQLTCHAR*)ConnStr, StateMsg, Msg, 1024);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}

	bool FilterSwitch = DataAccess::GetInstance()->GetFilterSwitch();

	SQLTCHAR SqlBuf[1024];
	StkPlSwPrintf((wchar_t*)SqlBuf, 1024, L"select count(*) from %ls", TableName);
	bool FirstCond = true;
	if (FilterSwitch) {
		for (int Loop = 1; Loop <= 5; Loop++) {
			if (StkPlWcsCmp(ColumnNameCnv[Loop - 1], L"\"*\"") == 0 || StkPlWcsCmp(ColumnNameCnv[Loop - 1], L"`*`") == 0 || OpeType[Loop - 1] == 0) {
				continue;
			}
			if (FirstCond == true) {
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" where ");
				FirstCond = false;
			} else {
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" and ");
			}
			StkPlWcsCat((wchar_t*)SqlBuf, 1024, ColumnNameCnv[Loop - 1]);
			switch (OpeType[Loop - 1]) {
			case 1:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" = "); break;
			case 2:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" <> "); break;
			case 3:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" <= "); break;
			case 4:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" < "); break;
			case 5:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" >= "); break;
			case 6:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" > "); break;
			case 10: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" like "); break;
			case 11: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" not like "); break;
			case 20: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" is null "); break;
			case 21: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" is not null "); break;
			}
			if (OpeType[Loop - 1] != 20 && OpeType[Loop - 1] != 21) {
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"'");
				if (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11) {
					StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"%");
				}
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, Value[Loop - 1]);
				if (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11) {
					StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"%");
				}
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"'");
			}
		}
	}
	StkPlWcsCat((wchar_t*)SqlBuf, 1024, L";");
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRecW(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return 0;
	}
	DWORD TmpNumOfRec;
	SQLBindCol(Hstmt, 1, SQL_C_SLONG, &TmpNumOfRec, 0, NULL);
	Ret = SQLFetch(Hstmt);

	Ret = CloseDatabase(StateMsg, Msg, 1024);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}
	return (int)TmpNumOfRec;
}

int DbAccessor::GetRecordsByTableNameCommon(SQLTCHAR* TableName,
	int NumOfCols, StkObject* Obj,
	wchar_t ColumnNameCnv[5][Global::COLUMNNAME_LENGTH * 4 + 2], int OpeType[5], wchar_t Value[5][Global::COLUMNVAL_LENGTH * 4 + 2],
	wchar_t SortTarget[Global::COLUMNNAME_LENGTH * 4 + 2],
	wchar_t SortOrder[5],
	int Limit, int Offset,
	SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	bool FilterSwitch = DataAccess::GetInstance()->GetFilterSwitch();

	SQLTCHAR SqlBuf[1024];
	StkPlLStrCpy((wchar_t*)SqlBuf, L"");

	StkPlSwPrintf((wchar_t*)SqlBuf, 1024, L"select * from %ls", TableName);
	bool FirstCond = true;
	if (FilterSwitch) {
		for (int Loop = 1; Loop <= 5; Loop++) {
			if (StkPlWcsCmp(ColumnNameCnv[Loop - 1], L"\"*\"") == 0 || StkPlWcsCmp(ColumnNameCnv[Loop - 1], L"`*`") == 0 || OpeType[Loop - 1] == 0) {
				continue;
			}
			if (FirstCond == true) {
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" where ");
				FirstCond = false;
			} else {
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" and ");
			}
			StkPlWcsCat((wchar_t*)SqlBuf, 1024, ColumnNameCnv[Loop - 1]);
			switch (OpeType[Loop - 1]) {
			case 1:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" = "); break;
			case 2:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" <> "); break;
			case 3:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" <= "); break;
			case 4:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" < "); break;
			case 5:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" >= "); break;
			case 6:  StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" > "); break;
			case 10: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" like "); break;
			case 11: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" not like "); break;
			case 20: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" is null "); break;
			case 21: StkPlWcsCat((wchar_t*)SqlBuf, 1024, L" is not null "); break;
			}
			if (OpeType[Loop - 1] != 20 && OpeType[Loop - 1] != 21) {
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"'");
				if (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11) {
					StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"%");
				}
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, Value[Loop - 1]);
				if (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11) {
					StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"%");
				}
				StkPlWcsCat((wchar_t*)SqlBuf, 1024, L"'");
			}
		}
	}
	if (SortTarget != NULL && *SortTarget != L'\0') {
		SQLTCHAR SqlSortBuf[128];
		StkPlLStrCpy((wchar_t*)SqlSortBuf, L"");
		StkPlSwPrintf((wchar_t*)SqlSortBuf, 128, L" order by %ls %ls", SortTarget, SortOrder);
		StkPlWcsCat((wchar_t*)SqlBuf, 1024, (wchar_t*)SqlSortBuf);
	}
	if (Limit != -1 && Offset != -1) {
		SQLTCHAR SqlLimitBuf[128];
		StkPlLStrCpy((wchar_t*)SqlLimitBuf, L"");
		StkPlSwPrintf((wchar_t*)SqlLimitBuf, 128, L" limit %d offset %d", Limit, Offset);
		StkPlWcsCat((wchar_t*)SqlBuf, 1024, (wchar_t*)SqlLimitBuf);
	}
	StkPlWcsCat((wchar_t*)SqlBuf, 1024, L";");
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRecW(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return 0;
	}

	SQLTCHAR TmpRecord[Global::MAXNUM_COLUMNS][Global::COLUMNVAL_LENGTH + 10]; // Why +10 ... MariaDB-Fetch returns data exceeding buffer size
	SQLLEN ValLen[Global::MAXNUM_COLUMNS];
	for (int LoopCol = 0; LoopCol < NumOfCols; LoopCol++) {
		SQLBindCol(Hstmt, LoopCol + 1, SQL_C_WCHAR, TmpRecord[LoopCol], (Global::COLUMNVAL_LENGTH - 1) * sizeof(SQLTCHAR), &ValLen[LoopCol]);
	}
	int LoopRec = 0;
	for (;;) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO) {
			SQLGetDiagRecW(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return 0;
		}
		StkObject* RecObj = new StkObject(L"Record");
		for (int LoopCol = 0; LoopCol < NumOfCols; LoopCol++) {
			wchar_t IndStr[5];
			StkPlSwPrintf(IndStr, 5, L"%d", LoopCol);
			if (ValLen[LoopCol] == -1) {
				RecObj->AppendChildElement(new StkObject(IndStr, L""));
			} else {
				TmpRecord[LoopCol][Global::COLUMNVAL_LENGTH - 1] = '\0';
				RecObj->AppendChildElement(new StkObject(IndStr, (wchar_t*)TmpRecord[LoopCol]));
			}
		}
		Obj->AppendChildElement(RecObj);
		LoopRec++;
	}
	return LoopRec;
}

SQLRETURN DbAccessor::OpenDatabase(SQLTCHAR* ConnectStr, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	StkPlLStrCpy((wchar_t*)Msg, L"");
	StkPlLStrCpy((wchar_t*)StateMsg, L"");

	// Alloc environment handle
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &Henv) == SQL_ERROR) {
		if (Henv != SQL_NULL_HENV) {
			SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		}
		return SQL_ERROR;
	}
	SQLSetEnvAttr(Henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);

	// Alloc DB connection handle
	if (SQLAllocHandle(SQL_HANDLE_DBC, Henv, &Hdbc) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	// SQLDriverConnect
	SQLTCHAR ConnOut[255]; // This will not be refered from anywhere
	SQLSMALLINT ConnOutLen; // This will not be refered from anywhere
	SQLRETURN Ret = SQLDriverConnectW(Hdbc, NULL, ConnectStr, SQL_NTS, ConnOut, 255, &ConnOutLen, SQL_DRIVER_COMPLETE);
	if (Ret == SQL_ERROR || Ret == SQL_SUCCESS_WITH_INFO) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return Ret;
	}

	// Alloc statement handle 
	if (SQLAllocHandle(SQL_HANDLE_STMT, Hdbc, &Hstmt) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	return SQL_SUCCESS;
}

SQLRETURN DbAccessor::CloseDatabase(SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	StkPlLStrCpy((wchar_t*)Msg, L"");
	StkPlLStrCpy((wchar_t*)StateMsg, L"");

	// Free statement handle
	if (SQLFreeHandle(SQL_HANDLE_STMT, Hstmt) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	// SQLDisconnect
	SQLRETURN Ret = SQLDisconnect(Hdbc);
	if (Ret == SQL_ERROR || Ret == SQL_SUCCESS_WITH_INFO) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return Ret;
	}

	// Free DB connection handle
	if (SQLFreeHandle(SQL_HANDLE_DBC, Hdbc) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	// Free environment handle
	if (SQLFreeHandle(SQL_HANDLE_ENV, Henv) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	return SQL_SUCCESS;
}
