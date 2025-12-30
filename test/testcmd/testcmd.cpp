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
	char* ChStateMsg = StkPlCreateUtf8FromWideChar(StateMsg);
	char* ChMsg = StkPlCreateUtf8FromWideChar(Msg);
#endif
	StkPlPrintf("An error occurred!\n%s\n%s\n", ChStateMsg, ChMsg);
	delete[] ChStateMsg;
	delete[] ChMsg;
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

	int ErrCode = 0;

	/////
	StkPlPrintf("Create table ... ");
	{
		StkObject* TableInfo = StkObject::CreateObjectFromJson(L"{\"test_table\" : {\"ColumnInfo\" : [{\"Name\":\"番号\", \"Type\":\"integer\"}, {\"Name\":\"prefecture\", \"Type\":\"varchar(10)\"}, {\"Name\":\"size\", \"Type\":\"varchar(10)\"}, {\"Name\":\"名称\", \"Type\":\"varchar(20)\"}, {\"Name\":\"age\", \"Type\":\"integer\"}, {\"Name\":\"memo\", \"Type\":\"varchar(80)\"}, {\"Name\":\"ポイント\", \"Type\":\"real\"}, {\"Name\":\"💛\", \"Type\":\"real\"}]}}", &ErrCode);
		if (DbAcc->CreateTable(TableInfo, StateMsg, Msg) != 0) {
			StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
			delete DbAcc;
			delete TableInfo;
			return -1;
		}
		delete TableInfo;
	}
	{
		const char32_t ChCol[15] = { U'🀄', U'🈁', U'🃏', U'𝝅', U'𝄢', U'𩸽', U'𠀋', U'𡈽', U'𥔎', U'𠮷', U'🌈', U'🔥', U'🚀', U'✨', U'🤝' };
		char32_t ColName[65] = U"";
		for (int ChIndex = 0; ChIndex < 16; ChIndex++) {
			ColName[ChIndex] = ChCol[StkPlRand() % 15];
		}
		wchar_t* ColNameWc = StkPlCreateWideCharFromUtf32(ColName);
		wchar_t JsonTxt[512] = L"";
		StkPlSwPrintf(JsonTxt, 512, L"{\"𠮷a♩あ🎵z☺\" : {\"ColumnInfo\" : [{\"Name\":\"◆_■\", \"Type\":\"varchar(80)\"}, {\"Name\":\"%ls\", \"Type\":\"varchar(80)\"}, {\"Name\":\"a\", \"Type\":\"integer\"}, {\"Name\":\"b\", \"Type\":\"integer\"}, {\"Name\":\"c\", \"Type\":\"integer\"}, {\"Name\":\"d\", \"Type\":\"integer\"}, {\"Name\":\"e\", \"Type\":\"integer\"}, {\"Name\":\"f\", \"Type\":\"integer\"}]}}", ColNameWc);
		delete ColNameWc;

		StkObject* TableInfo = StkObject::CreateObjectFromJson(JsonTxt, &ErrCode);
		if (DbAcc->CreateTable(TableInfo, StateMsg, Msg) != 0) {
			StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
			delete DbAcc;
			delete TableInfo;
			return -1;
		}
		delete TableInfo;
	}
	{
		StkObject* Root = new StkObject(L"");
		StkObject* TableInfo = new StkObject(L"✨✨✨✨🔥🔥🔥🔥🌈🌈🌈🌈🚀🚀🚀🚀");
		StkObject* ColumnInfo = NULL;
		for (int Loop = 0; Loop < 60; Loop++) {
			wchar_t Name[10] = { 0 };
			StkPlSwPrintf(Name, 10, L"Col%02d", Loop);
			StkObject* ColumnName = new StkObject(L"Name", Name);
			StkObject* ColumnType = new StkObject(L"Type", L"integer");
			ColumnName->SetNext(ColumnType);
			ColumnInfo = new StkObject(L"ColumnInfo");
			ColumnInfo->AppendChildElement(ColumnName);
			TableInfo->AppendChildElement(ColumnInfo);
		}
		Root->AppendChildElement(TableInfo);

		if (DbAcc->CreateTable(Root, StateMsg, Msg) != 0) {
			StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
			delete DbAcc;
			delete Root;
			return -1;
		}
		delete Root;
	}
	StkPlPrintf("OK\r\n");
	
	/////
	StkPlPrintf("Insert records ... ");
	{
		const wchar_t* Pref[10] = { L"静岡", L"石川", L"神奈川", L"愛知", L"北海道", L"東京", L"沖縄", L"三重", L"京都", L"青森" };
		const wchar_t* Size[3] = { L"大規模", L"中規模", L"小規模" };
		wchar_t Chmemo[15] = { L'松', L'a', L'竹', L'7', L'梅', L'古', L'今', L'東', L'西', L'x', L'\'', L'\"', L',', L';', L'`'};
		for (int Page = 0; Page < 5; Page++) {
			StkObject* Root = new StkObject(L"");
			for (int Loop = 0; Loop < 30; Loop++) {
				StkObject* Table = new StkObject(L"test_table");
				StkObject* Rec0 = new StkObject(L"RecordInfo", Page * 30 + Loop);
				StkObject* Rec1 = new StkObject(L"RecordInfo", Pref[StkPlRand() % 10]);
				StkObject* Rec2 = new StkObject(L"RecordInfo", Size[StkPlRand() % 3]);
				StkObject* Rec3 = new StkObject(L"RecordInfo", L"𠮷野家;♩♩♩;💛💛💛;👉👉👉");
				StkObject* Rec4 = new StkObject(L"RecordInfo", StkPlRand() % 500);
				wchar_t Memo[80] = L"";
				for (int ChIndex = 0; ChIndex < 75; ChIndex++) {
					Memo[ChIndex] = Chmemo[StkPlRand() % 15];
				}
				Memo[75] = L'\0';
				StkObject* Rec5 = new StkObject(L"RecordInfo", Memo);
				StkObject* Rec6 = new StkObject(L"RecordInfo", (float)(StkPlRand() % 1000) / 100.0f);
				StkObject* Rec7 = new StkObject(L"RecordInfo", (float)(StkPlRand() % 100) / 1000.0f);
				Table->AppendChildElement(Rec0);
				Table->AppendChildElement(Rec1);
				Table->AppendChildElement(Rec2);
				Table->AppendChildElement(Rec3);
				Table->AppendChildElement(Rec4);
				Table->AppendChildElement(Rec5);
				Table->AppendChildElement(Rec6);
				Table->AppendChildElement(Rec7);
				Root->AppendChildElement(Table);
			}
			if (DbAcc->InsertRecord(Root, StateMsg, Msg) != 0) {
				StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
				delete DbAcc;
				delete Root;
				return -1;
			}
			delete Root;
		}
	}
	{
		wchar_t Chmemo[15] = { L'松', L'a', L'竹', L'7', L'梅', L'古', L'今', L'東', L'西', L'x', L'\'', L'\"', L',', L';', L'`' };
		for (int Page = 0; Page < 500; Page++) {
			StkObject* Root = new StkObject(L"");
			for (int Loop = 0; Loop < 30; Loop++) {
				StkObject* Table = new StkObject(L"𠮷a♩あ🎵z☺");
				wchar_t Memo[80] = L"";
				for (int ChIndex = 0; ChIndex < 75; ChIndex++) {
					Memo[ChIndex] = Chmemo[StkPlRand() % 15];
				}
				StkObject* Rec0 = new StkObject(L"RecordInfo", Memo);
				StkObject* Rec1 = new StkObject(L"RecordInfo", Memo);
				Table->AppendChildElement(Rec0);
				Table->AppendChildElement(Rec1);
				Table->AppendChildElement(new StkObject(L"RecordInfo", Page * 30 + Loop));
				Table->AppendChildElement(new StkObject(L"RecordInfo", 0));
				Table->AppendChildElement(new StkObject(L"RecordInfo", 0));
				Table->AppendChildElement(new StkObject(L"RecordInfo", 0));
				Table->AppendChildElement(new StkObject(L"RecordInfo", 0));
				Table->AppendChildElement(new StkObject(L"RecordInfo", 0));
				Root->AppendChildElement(Table);
			}
			if (DbAcc->InsertRecord(Root, StateMsg, Msg) != 0) {
				StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
				delete DbAcc;
				delete Root;
				return -1;
			}
			delete Root;
		}
	}
	{
		for (int Page = 0; Page < 15; Page++) {
			StkObject* Root = new StkObject(L"");
			for (int Loop = 0; Loop < 10; Loop++) {
				StkObject* Table = new StkObject(L"✨✨✨✨🔥🔥🔥🔥🌈🌈🌈🌈🚀🚀🚀🚀");
				for (int ChIndex = 0; ChIndex < 60; ChIndex++) {
					Table->AppendChildElement(new StkObject(L"RecordInfo", StkPlRand() % 1000));
				}
				Root->AppendChildElement(Table);
			}
			if (DbAcc->InsertRecord(Root, StateMsg, Msg) != 0) {
				StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
				delete DbAcc;
				delete Root;
				return -1;
			}
			delete Root;
		}
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
	if (DbAcc->DropTable(L"𠮷a♩あ🎵z☺", StateMsg, Msg) != 0) {
		StkPlWPrintf(L"NG %ls : %ls\r\n", StateMsg, Msg);
		delete DbAcc;
		return -1;
	}
	if (DbAcc->DropTable(L"✨✨✨✨🔥🔥🔥🔥🌈🌈🌈🌈🚀🚀🚀🚀", StateMsg, Msg) != 0) {
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

	if (StkPlStrCmp(argv[3], "GENERAL") == 0 && TestGeneral(OdbcConStr, DbmsType) != 0) {
		//
	}
	if (StkPlStrCmp(argv[3], "CLEANUP") == 0 && TestCleanup(OdbcConStr, DbmsType) != 0) {
		//
	}

	delete[] Dbms;
	delete[] OdbcConStr;
	delete[] Scenario;
	return 0;
}
