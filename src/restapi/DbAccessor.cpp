#include <windows.h>
#include <stdio.h>
#include "DbAccessor.h"
#include "..\Global.h"
#include "dataaccess.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkObject.h"

DbAccessor::DbAccessor()
{
}

DbAccessor::~DbAccessor()
{
}

int DbAccessor::Test(TCHAR ConnStr[Global::MAX_PARAM_LENGTH])
{
	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];
	int Ret = 0;
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, 1024);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}
	Ret = CloseDatabase(StateMsg, Msg, 1024);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}
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
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return Ret;
	}
	SQLTCHAR TableName[Global::TABLENAME_LENGTH];
	SQLBindCol(Hstmt, 1, SQL_C_WCHAR, TableName, Global::TABLENAME_LENGTH * sizeof(SQLTCHAR), NULL);

	BOOL InitFlag = TRUE;
	int Loop = 0;
	for (;;) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO) {
			SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return Ret;
		}
		StkObject* TblInfObj = new StkObject(L"TableInfo");
		TblInfObj->AppendChildElement(new StkObject(L"Name", (TCHAR*)TableName));
		Obj->AppendChildElement(TblInfObj);
		Loop++;
		if (Loop >= Global::MAXNUM_TABLES) {
			break;
		}
	}

	return Ret;
}

int DbAccessor::GetNumOfRecordsCommon(SQLTCHAR* TableName, SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	Ret = OpenDatabase(ConnStr, StateMsg, Msg, 1024);
	if (Ret != SQL_SUCCESS) {
		return Ret;
	}

	SQLTCHAR SqlBuf[1024];
	_snwprintf_s(SqlBuf, 1024, _TRUNCATE, L"select count(*) from %s;", TableName);
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
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
	TCHAR ColumnNameCnv[5][Global::COLUMNNAME_LENGTH * 4 + 2], int OpeType[5], TCHAR Value[5][Global::COLUMNVAL_LENGTH * 4 + 2],
	SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	SQLRETURN Ret = 0;

	BOOL FilterSwitch = DataAccess::GetInstance()->GetFilterSwitch();

	SQLTCHAR SqlBuf[1024];
	_snwprintf_s(SqlBuf, 1024, _TRUNCATE, L"select * from %s", TableName);
	BOOL FirstCond = TRUE;
	if (FilterSwitch) {
		for (int Loop = 1; Loop <= 5; Loop++) {
			if (lstrcmp(ColumnNameCnv[Loop - 1], L"\"*\"") == 0 || lstrcmp(ColumnNameCnv[Loop - 1], L"`*`") == 0 || OpeType[Loop - 1] == 0) {
				continue;
			}
			if (FirstCond == TRUE) {
				lstrcat(SqlBuf, L" where ");
				FirstCond = FALSE;
			} else {
				lstrcat(SqlBuf, L" and ");
			}
			lstrcat(SqlBuf, ColumnNameCnv[Loop - 1]);
			switch (OpeType[Loop - 1]) {
			case 1:  lstrcat(SqlBuf, L" = "); break;
			case 2:  lstrcat(SqlBuf, L" <> "); break;
			case 3:  lstrcat(SqlBuf, L" <= "); break;
			case 4:  lstrcat(SqlBuf, L" < "); break;
			case 5:  lstrcat(SqlBuf, L" >= "); break;
			case 6:  lstrcat(SqlBuf, L" > "); break;
			case 10: lstrcat(SqlBuf, L" like "); break;
			case 11: lstrcat(SqlBuf, L" not like "); break;
			case 20: lstrcat(SqlBuf, L" is null "); break;
			case 21: lstrcat(SqlBuf, L" is not null "); break;
			}
			if (OpeType[Loop - 1] != 20 && OpeType[Loop - 1] != 21) {
				lstrcat(SqlBuf, L"'");
				if (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11) {
					lstrcat(SqlBuf, L"%");
				}
				lstrcat(SqlBuf, Value[Loop - 1]);
				if (OpeType[Loop - 1] == 10 || OpeType[Loop - 1] == 11) {
					lstrcat(SqlBuf, L"%");
				}
				lstrcat(SqlBuf, L"'");
			}
		}
	}
	lstrcat(SqlBuf, L";");
	Ret = SQLExecDirect(Hstmt, SqlBuf, SQL_NTS);
	if (Ret != SQL_SUCCESS) {
		SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return 0;
	}

	SQLTCHAR TmpRecord[Global::MAXNUM_COLUMNS][Global::COLUMNVAL_LENGTH];
	SQLLEN ValLen[Global::MAXNUM_COLUMNS];
	for (int LoopCol = 0; LoopCol < NumOfCols; LoopCol++) {
		SQLBindCol(Hstmt, LoopCol + 1, SQL_C_WCHAR, TmpRecord[LoopCol], Global::COLUMNVAL_LENGTH * sizeof(SQLTCHAR), &ValLen[LoopCol]);
	}
	int LoopRec = 0;
	for (;;) {
		Ret = SQLFetch(Hstmt);
		if (Ret == SQL_NO_DATA_FOUND) break;
		if (Ret != SQL_SUCCESS && Ret != SQL_SUCCESS_WITH_INFO) {
			SQLGetDiagRec(SQL_HANDLE_STMT, Hstmt, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
			return 0;
		}
		StkObject* RecObj = new StkObject(L"Record");
		for (int LoopCol = 0; LoopCol < NumOfCols; LoopCol++) {
			TCHAR IndStr[5];
			_snwprintf_s(IndStr, 5, _TRUNCATE, L"%d", LoopCol);
			if (ValLen[LoopCol] == -1) {
				RecObj->AppendChildElement(new StkObject(IndStr, L""));
			} else {
				TmpRecord[LoopCol][Global::COLUMNVAL_LENGTH - 1] = '\0';
				RecObj->AppendChildElement(new StkObject(IndStr, TmpRecord[LoopCol]));
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
	lstrcpy(Msg, L"");
	lstrcpy(StateMsg, L"");

	// Alloc environment handle
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &Henv) == SQL_ERROR) {
		if (Henv != SQL_NULL_HENV) {
			SQLGetDiagRec(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		}
		return SQL_ERROR;
	}
	SQLSetEnvAttr(Henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);

	// Alloc DB connection handle
	if (SQLAllocHandle(SQL_HANDLE_DBC, Henv, &Hdbc) == SQL_ERROR) {
		SQLGetDiagRec(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	// SQLDriverConnect
	SQLTCHAR ConnOut[255]; // This will not be refered from anywhere
	SQLSMALLINT ConnOutLen; // This will not be refered from anywhere
	SQLRETURN Ret = SQLDriverConnect(Hdbc, NULL, ConnectStr, SQL_NTS, ConnOut, 255, &ConnOutLen, SQL_DRIVER_COMPLETE);
	if (Ret == SQL_ERROR || Ret == SQL_SUCCESS_WITH_INFO) {
		SQLGetDiagRec(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return Ret;
	}

	// Alloc statement handle 
	if (SQLAllocHandle(SQL_HANDLE_STMT, Hdbc, &Hstmt) == SQL_ERROR) {
		SQLGetDiagRec(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	return SQL_SUCCESS;
}

SQLRETURN DbAccessor::CloseDatabase(SQLTCHAR StateMsg[10], SQLTCHAR* Msg, SQLSMALLINT MsgLen)
{
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	lstrcpy(Msg, L"");
	lstrcpy(StateMsg, L"");

	// Free statement handle
	if (SQLFreeHandle(SQL_HANDLE_STMT, Hstmt) == SQL_ERROR) {
		SQLGetDiagRec(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	// SQLDisconnect
	SQLRETURN Ret = SQLDisconnect(Hdbc);
	if (Ret == SQL_ERROR || Ret == SQL_SUCCESS_WITH_INFO) {
		SQLGetDiagRec(SQL_HANDLE_DBC, Hdbc, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return Ret;
	}

	// Free DB connection handle
	if (SQLFreeHandle(SQL_HANDLE_DBC, Hdbc) == SQL_ERROR) {
		SQLGetDiagRec(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	// Free environment handle
	if (SQLFreeHandle(SQL_HANDLE_ENV, Henv) == SQL_ERROR) {
		SQLGetDiagRec(SQL_HANDLE_ENV, Henv, 1, StateMsg, &Native, Msg, MsgLen, &ActualMsgLen);
		return SQL_ERROR;
	}

	return SQL_SUCCESS;
}
