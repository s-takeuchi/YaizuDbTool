#ifdef WIN32
	#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>

#include "../../../YaizuComLib/src/stkpl/StkPl.h"

#define POSTGRESQL 1
#define MYSQL 2
#define MARIADB 3

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

void ShowErrorMsg(wchar_t StateMsg[10], wchar_t Msg[1024])
{
#ifdef WIN32
	char* ChStateMsg = StkPlWideCharToSjis(StateMsg);
	char* ChMsg = StkPlWideCharToSjis(Msg);
#else
	char* ChStateMsg = StkPlWideCharToUtf8(StateMsg);
	char* ChMsg = StkPlWideCharToUtf8(Msg);
#endif
	StkPlPrintf("An error occurred!\n%s\n%s\n", ChStateMsg, ChMsg);
}

int TestGeneral(wchar_t* OdbcConStr)
{
	int Ret = 0;
	wchar_t StateMsg[10] = L"";
	wchar_t Msg[1024] = L"";
	Ret = OpenDatabase(OdbcConStr, StateMsg, Msg);
	if (Ret != 0) {
		ShowErrorMsg(StateMsg, Msg);
		return -1;
	}
	Ret = CloseDatabase(StateMsg, Msg);
	if (Ret != 0) {
		ShowErrorMsg(StateMsg, Msg);
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
		wchar_t CmdName[64] = L"";
		wchar_t* PathToCmd = StkPlCreateWideCharFromUtf8(argv[0]);
		StkPlGetFileNameFromFullPath(PathToCmd, CmdName, 64);
		delete[] PathToCmd;
		StkPlWPrintf(L"Usage: %ls dbms odbc_connection_string test_scenario\n", CmdName);
		StkPlPrintf("dbms:\n");
		StkPlPrintf("    postgresql, mysql or mariadb\n");
		StkPlPrintf("test_scenario:\n");
		StkPlPrintf("    GENERAL ... 3 tables, 8 columns and 150 records in each table.\n");
		StkPlPrintf("    MAX_TABLES ... 300 tables, 8 columns and 150 records in each table.\n");
		StkPlPrintf("\n");
		StkPlPrintf("ex. %ls postgresql \"Driver={PostgreSQL};Server=127.0.0.1;Database=testdb;UID=admin;PWD=admin;Port=5432;\" GENERAL\n", CmdName);
		StkPlExit(0);
	}
	StkPlPrintf("dbms = %s\n", argv[1]);
	StkPlPrintf("odbc_connection_string = %s\n", argv[2]);
	StkPlPrintf("test_scenario = %s\n", argv[3]);
#ifdef WIN32
	wchar_t* Dbms = StkPlSjisToWideChar(argv[1]);
	wchar_t* OdbcConStr = StkPlSjisToWideChar(argv[2]);
	wchar_t* Scenario = StkPlSjisToWideChar(argv[3]);
#else
	wchar_t* Dbms = StkPlCreateWideCharFromUtf8(argv[1]);
	wchar_t* OdbcConStr = StkPlCreateWideCharFromUtf8(argv[2]);
	wchar_t* Scenario = StkPlCreateWideCharFromUtf8(argv[3]);
#endif

	int DbmsType = 0;
	if (StkPlWcsCmp(Dbms, L"postgresql") == 0) {
		DbmsType = POSTGRESQL;
	} else if (StkPlWcsCmp(Dbms, L"mysql") == 0) {
		DbmsType = MYSQL;
	} else if (StkPlWcsCmp(Dbms, L"mariadb") == 0) {
		DbmsType = MARIADB;
	}

	if (TestGeneral(OdbcConStr) != 0) {
		//
	}

	delete[] Dbms;
	delete[] OdbcConStr;
	delete[] Scenario;
	return 0;
}
