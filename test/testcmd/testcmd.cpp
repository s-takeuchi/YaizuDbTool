#ifdef WIN32
	#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>

#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkdb/DbAccessor.h"
#include "../../../YaizuComLib/src/stkdb/DbPostgreSqlAccessor.h"
#include "../../../YaizuComLib/src/stkdb/DbMySqlAccessor.h"
#include "../../../YaizuComLib/src/stkdb/DbMariaDbAccessor.h"

#define POSTGRESQL 1
#define MYSQL 2
#define MARIADB 3

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

int TestGeneral(wchar_t* OdbcConStr, int DbmsType)
{
	int Ret = 0;
	wchar_t StateMsg[10] = L"";
	wchar_t Msg[1024] = L"";
	DbAccessor* DbAcc = NULL;
	switch (DbmsType) {
	case POSTGRESQL:
		DbAcc = new DbPostgreSqlAccessor(OdbcConStr);
		break;
	case MYSQL:
		DbAcc = new DbMySqlAccessor(OdbcConStr);
		break;
	case MARIADB:
		DbAcc = new DbMariaDbAccessor(OdbcConStr);
		break;
	default:
		break;
	}

	/////
	StkPlPrintf("Test connection ... ");
	Ret = DbAcc->Test(StateMsg, Msg);
	if (Ret != 0) {
		StkPlPrintf("NG\r\n");
		ShowErrorMsg(StateMsg, Msg);
		delete DbAcc;
		return -1;
	}
	StkPlPrintf("OK\r\n");

	/////
	StkPlPrintf("Create table ... ");
	StkObject* TableObj = new StkObject(L"test_table");
	StkObject* ColumnObj1 = new StkObject(L"ColumnInfo");
	ColumnObj1->AppendChildElement(new StkObject(L"Name", L"aaa"));
	ColumnObj1->AppendChildElement(new StkObject(L"Type", L"integer"));
	StkObject* ColumnObj2 = new StkObject(L"ColumnInfo");
	ColumnObj2->AppendChildElement(new StkObject(L"Name", L"bbb"));
	ColumnObj2->AppendChildElement(new StkObject(L"Type", L"integer"));
	TableObj->AppendChildElement(ColumnObj1);
	TableObj->AppendChildElement(ColumnObj2);
	if (DbAcc->CreateTable(TableObj, StateMsg, Msg) != 0) {
		StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
		delete DbAcc;
		return -1;
	}
	StkPlPrintf("OK\r\n");
	
	delete DbAcc;
	return 0;
}

int TestCleanup(wchar_t* OdbcConStr, int DbmsType)
{
	wchar_t StateMsg[10] = L"";
	wchar_t Msg[1024] = L"";
	DbAccessor* DbAcc = NULL;
	switch (DbmsType) {
	case POSTGRESQL:
		DbAcc = new DbPostgreSqlAccessor(OdbcConStr);
		break;
	case MYSQL:
		DbAcc = new DbMySqlAccessor(OdbcConStr);
		break;
	case MARIADB:
		DbAcc = new DbMariaDbAccessor(OdbcConStr);
		break;
	default:
		break;
	}

	/////
	StkPlPrintf("Drop table ... ");
	if (DbAcc->DropTable(L"test_table", StateMsg, Msg) != 0) {
		StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
		delete DbAcc;
		return -1;
	}
	StkPlPrintf("OK\r\n");

	delete DbAcc;
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
		StkPlPrintf("    CLEANUP ... Drop all tables which were created in this test command.\n");
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

	if (strcmp(argv[3], "GENERAL") == 0 && TestGeneral(OdbcConStr, DbmsType) != 0) {
		//
	}
	if (strcmp(argv[3], "CLEANUP") == 0 && TestCleanup(OdbcConStr, DbmsType) != 0) {
		//
	}

	delete[] Dbms;
	delete[] OdbcConStr;
	delete[] Scenario;
	return 0;
}
