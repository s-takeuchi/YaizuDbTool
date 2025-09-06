#ifdef WIN32
	#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>

#include "../../../YaizuComLib/src/stkpl/StkPl.h"

SQLHENV  Henv;
SQLHDBC  Hdbc;
SQLHSTMT Hstmt;


void ConvertMessage(wchar_t StateMsg[10], wchar_t Msg[1024], const char16_t SqlStateMsg[10], const char16_t SqlMsg[1024])
{
	StkPlConvUtf16ToWideChar(StateMsg, 10, SqlStateMsg);
	StkPlConvUtf16ToWideChar(Msg, 1024, SqlMsg);
}

// Return 0: Success, -1:Error
int OpenDatabase(wchar_t* ConnectStr, wchar_t StateMsg[10], wchar_t Msg[1024])
{
	SQLWCHAR CvtConnectStr[256];
	StkPlConvWideCharToUtf16((char16_t*)CvtConnectStr, 256, ConnectStr);

	SQLWCHAR CvtStateMsg[10];
	SQLWCHAR CvtMsg[1024];
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	StkPlLStrCpy(Msg, L"");
	StkPlLStrCpy(StateMsg, L"");

	// Alloc environment handle
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &(Henv)) == SQL_ERROR) {
		if (Henv != SQL_NULL_HENV) {
			SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		}
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}
	SQLSetEnvAttr(Henv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0);

	// Alloc DB connection handle
	if (SQLAllocHandle(SQL_HANDLE_DBC, Henv, &Hdbc) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}

	// SQLDriverConnect
	SQLWCHAR ConnOut[255]; // This will not be refered from anywhere
	SQLSMALLINT ConnOutLen; // This will not be refered from anywhere
	SQLRETURN Ret = SQLDriverConnectW(Hdbc, NULL, CvtConnectStr, SQL_NTS, ConnOut, 255, &ConnOutLen, SQL_DRIVER_COMPLETE);
	if (Ret == SQL_ERROR || Ret == SQL_SUCCESS_WITH_INFO) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}

	// Alloc statement handle 
	if (SQLAllocHandle(SQL_HANDLE_STMT, Hdbc, &Hstmt) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}

	return 0;
}

// Return 0: Success, -1:Error
int CloseDatabase(wchar_t StateMsg[10], wchar_t Msg[1024])
{
	SQLWCHAR CvtStateMsg[10];
	SQLWCHAR CvtMsg[1024];
	SQLINTEGER Native; // This will not be refered from anywhere
	SQLSMALLINT ActualMsgLen; // This will not be refered from anywhere
	StkPlLStrCpy((wchar_t*)Msg, L"");
	StkPlLStrCpy((wchar_t*)StateMsg, L"");

	// Free statement handle
	if (SQLFreeHandle(SQL_HANDLE_STMT, Hstmt) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}

	// SQLDisconnect
	SQLRETURN Ret = SQLDisconnect(Hdbc);
	if (Ret == SQL_ERROR || Ret == SQL_SUCCESS_WITH_INFO) {
		SQLGetDiagRecW(SQL_HANDLE_DBC, Hdbc, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}

	// Free DB connection handle
	if (SQLFreeHandle(SQL_HANDLE_DBC, Hdbc) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}

	// Free environment handle
	if (SQLFreeHandle(SQL_HANDLE_ENV, Henv) == SQL_ERROR) {
		SQLGetDiagRecW(SQL_HANDLE_ENV, Henv, 1, CvtStateMsg, &Native, CvtMsg, 1024, &ActualMsgLen);
		ConvertMessage(StateMsg, Msg, (char16_t*)CvtStateMsg, (char16_t*)CvtMsg);
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		wchar_t CmdName[64] = L"";
		wchar_t* PathToCmd = StkPlCreateWideCharFromUtf8(argv[0]);
		StkPlGetFileNameFromFullPath(PathToCmd, CmdName, 64);
		delete[] PathToCmd;
		StkPlWPrintf(L"Usage: %ls odbc_connection_string test_scenario\n", CmdName);
		StkPlPrintf("test_scenario:\n");
		StkPlPrintf("    GENERAL ... 3 tables, 8 columns and 150 records in each table.\n");
		StkPlPrintf("    MAX_TABLES ... 300 tables, 8 columns and 150 records in each table.\n");
		StkPlExit(0);
	}
	
	return 0;
}
