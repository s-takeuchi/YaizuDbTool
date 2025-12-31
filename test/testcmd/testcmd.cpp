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
	wchar_t BufTxt[1024] = L"";
	StkPlSwPrintf(BufTxt, 1024, L"\nAn error occurred!\n%ls\n%ls\n", StateMsg, Msg);
#ifdef WIN32
	char* Out = StkPlWideCharToSjis(BufTxt);
	StkPlPrintf(Out);
	delete[] Out;
#else
	char* ChStateMsg = StkPlCreateUtf8FromWideChar(StateMsg);
	char* ChMsg = StkPlCreateUtf8FromWideChar(Msg);
	StkPlPrintf("NG\nAn error occurred!\n%s\n%s\n", ChStateMsg, ChMsg);
	delete[] ChStateMsg;
	delete[] ChMsg;
#endif
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
			ShowErrorMsg(StateMsg, Msg);
			delete DbAcc;
			delete TableInfo;
			return -1;
		}
		delete TableInfo;
	}
	{
		const char32_t ChCol[15] = { U'🀄', U'🈁', U'🃏', U'𝝅', U'𝄢', U'𩸽', U'𠀋', U'𡈽', U'𥔎', U'𠮷', U'🌈', U'🔥', U'🚀', U'🀅', U'🤝' };
		char32_t ColName[65] = U"";
		for (int ChIndex = 0; ChIndex < 15; ChIndex++) {
			ColName[ChIndex] = ChCol[StkPlRand() % 15];
		}
		wchar_t* ColNameWc = StkPlCreateWideCharFromUtf32(ColName);
		wchar_t JsonTxt[512] = L"";
		StkPlSwPrintf(JsonTxt, 512, L"{\"𠮷a♩あ🎵z☺\" : {\"ColumnInfo\" : [{\"Name\":\"◆_■\", \"Type\":\"varchar(80)\"}, {\"Name\":\"%ls\", \"Type\":\"varchar(80)\"}, {\"Name\":\"a\", \"Type\":\"integer\"}, {\"Name\":\"b\", \"Type\":\"integer\"}, {\"Name\":\"c\", \"Type\":\"integer\"}, {\"Name\":\"d\", \"Type\":\"integer\"}, {\"Name\":\"e\", \"Type\":\"integer\"}, {\"Name\":\"f\", \"Type\":\"integer\"}]}}", ColNameWc);
		delete ColNameWc;

		StkObject* TableInfo = StkObject::CreateObjectFromJson(JsonTxt, &ErrCode);
		if (DbAcc->CreateTable(TableInfo, StateMsg, Msg) != 0) {
			ShowErrorMsg(StateMsg, Msg);
			delete DbAcc;
			delete TableInfo;
			return -1;
		}
		delete TableInfo;
	}
	{
		StkObject* Root = new StkObject(L"");
		StkObject* TableInfo = new StkObject(L"🀅🀅🀅🔥🔥🔥🔥🌈🌈🌈🌈🚀🚀🚀🚀");
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
			ShowErrorMsg(StateMsg, Msg);
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
				ShowErrorMsg(StateMsg, Msg);
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
				ShowErrorMsg(StateMsg, Msg);
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
				StkObject* Table = new StkObject(L"🀅🀅🀅🔥🔥🔥🔥🌈🌈🌈🌈🚀🚀🚀🚀");
				for (int ChIndex = 0; ChIndex < 60; ChIndex++) {
					Table->AppendChildElement(new StkObject(L"RecordInfo", StkPlRand() % 1000));
				}
				Root->AppendChildElement(Table);
			}
			if (DbAcc->InsertRecord(Root, StateMsg, Msg) != 0) {
				ShowErrorMsg(StateMsg, Msg);
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

int TestSeService(wchar_t* OdbcConStr, int DbmsType)
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
		ShowErrorMsg(StateMsg, Msg);
		delete DbAcc;
		return -1;
	}
	StkPlPrintf("OK\r\n");

	int ErrCode = 0;

	/////
	StkPlPrintf("Create table ... ");
	{
		StkObject* TableInfo = StkObject::CreateObjectFromJson(L"{\"system_engineer\" : {\"ColumnInfo\" : [{\"Name\":\"id\", \"Type\":\"integer\"}, {\"Name\":\"name\", \"Type\":\"varchar(12)\"}, {\"Name\":\"start_year\", \"Type\":\"integer\"}, {\"Name\":\"age\", \"Type\":\"integer\"}, {\"Name\":\"experience_dev\", \"Type\":\"varchar(20)\"}, {\"Name\":\"experience_mgr\", \"Type\":\"varchar(20)\"}, {\"Name\":\"experience_ope\", \"Type\":\"varchar(20)\"}, {\"Name\":\"experience_cst\", \"Type\":\"varchar(20)\"}]}}", &ErrCode);
		if (DbAcc->CreateTable(TableInfo, StateMsg, Msg) != 0) {
			ShowErrorMsg(StateMsg, Msg);
			delete DbAcc;
			delete TableInfo;
			return -1;
		}
		delete TableInfo;
	}
	{
		StkObject* TableInfo = StkObject::CreateObjectFromJson(L"{\"assignment\" : {\"ColumnInfo\" : [{\"Name\":\"id\", \"Type\":\"integer\"}, {\"Name\":\"name\", \"Type\":\"varchar(30)\"}, {\"Name\":\"description\", \"Type\":\"varchar(30)\"}, {\"Name\":\"revenue\", \"Type\":\"integer\"}]}}", &ErrCode);
		if (DbAcc->CreateTable(TableInfo, StateMsg, Msg) != 0) {
			ShowErrorMsg(StateMsg, Msg);
			delete DbAcc;
			delete TableInfo;
			return -1;
		}
		delete TableInfo;
	}
	StkPlPrintf("OK\r\n");

	/////
	StkPlPrintf("Insert records ... ");
	{
		const wchar_t* LastName[32] = { L"佐藤", L"鈴木", L"高橋", L"田中", L"伊藤", L"渡辺", L"山本", L"中村",
										L"小林", L"加藤", L"吉田", L"山田", L"佐々木", L"山口", L"松本", L"井上",
										L"木村", L"林", L"斉藤", L"清水", L"山崎", L"森", L"池田", L"橋本",
										L"阿部", L"石川", L"山下", L"中島", L"小川", L"前田", L"岡田", L"長谷川"
		};
		const wchar_t* FirstName[32] = { L"博", L"茂", L"誠", L"大輔", L"翔太", L"拓也", L"健太", L"翔",
										L"樹", L"海", L"翼", L"大輝", L"悠斗", L"悠希", L"蓮", L"蒼",
										L"和子", L"幸子", L"恵子", L"久美子", L"由美子", L"真由美", L"明美", L"直美",
										L"愛", L"美咲", L"成美", L"紀子", L"雅子", L"さくら", L"陽菜", L"結"
		};
		const wchar_t* ExDev[5] = { L"経験なし", L"Back-endの開発経験あり", L"Front-endの開発経験あり", L"インフラ構築経験あり", L"フルスタック" };
		const wchar_t* ExMgr[5] = { L"経験なし", L"5名程度のチーム運営",  L"10名程度のチーム運営", L"20名程度のチーム運営", L"50名程度のチーム運営" };
		const wchar_t* ExOpe[4] = { L"経験なし", L"小規模サービス運用経験", L"中規模サービス運用経験", L"大規模サービス運用経験" };
		const wchar_t* ExCst[4] = { L"経験なし", L"要件定義", L"上流設計", L"要件定義＋上流設計"};
		for (int Page = 0; Page < 30; Page++) {
			StkObject* Root = new StkObject(L"");
			for (int Loop = 0; Loop < 30; Loop++) {
				wchar_t NameBuf[10] = L"";
				int RndF = StkPlRand() % 32;
				int RndL = StkPlRand() % 32;
				int StartYear = 1985 + (StkPlRand() % 20);
				int Age = 2025 - StartYear + 20 + (StkPlRand()) % 4;
				StkPlSwPrintf(NameBuf, 10, L"%ls %ls", LastName[RndF], FirstName[RndL]);
				StkObject* Table = new StkObject(L"system_engineer");
				StkObject* Rec0 = new StkObject(L"RecordInfo", Page * 30 + Loop + 1);
				StkObject* Rec1 = new StkObject(L"RecordInfo", NameBuf);
				StkObject* Rec2 = new StkObject(L"RecordInfo", StartYear);
				StkObject* Rec3 = new StkObject(L"RecordInfo", Age);
				StkObject* Rec4 = new StkObject(L"RecordInfo", ExDev[StkPlRand() % 5]);
				StkObject* Rec5 = new StkObject(L"RecordInfo", ExMgr[StkPlRand() % 5]);
				StkObject* Rec6 = new StkObject(L"RecordInfo", ExOpe[StkPlRand() % 4]);
				StkObject* Rec7 = new StkObject(L"RecordInfo", ExCst[StkPlRand() % 4]);
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
				ShowErrorMsg(StateMsg, Msg);
				delete DbAcc;
				delete Root;
				return -1;
			}
			delete Root;
		}
	}
	{
		const wchar_t* Title[6] = { L"ABC銀行", L"M自治体", L"XYZ保険", L"QQQ信用金庫", L"N自治体", L"P生命保険" };
		const wchar_t* System[6] = { L"Aシステム", L"Bシステム", L"Cシステム", L"Dシステム", L"Eシステム", L"Fシステム" };
		const wchar_t* Subtitle[6] = { L"データベース移行", L"インフラ刷新", L"Front-end改善", L"Back-end改善", L"システム機能拡充", L"システム統合" };
		const wchar_t* Desc1[5] = { L"5名×3カ月;", L"5名×6カ月;", L"10名×6カ月;", L"10名×12カ月;", L"先行検討3名×3カ月;" };
		const wchar_t* Desc2[2] = { L"難易度高;", L"難易度中;" };
		const wchar_t* Desc3[2] = { L"単発案件", L"次期案件獲得の可能性あり" };
		for (int Page = 0; Page < 2; Page++) {
			StkObject* Root = new StkObject(L"");
			for (int Loop = 0; Loop < 30; Loop++) {
				wchar_t NameBuf[30] = L"";
				StkPlSwPrintf(NameBuf, 30, L"%ls %ls %ls", Title[StkPlRand() % 6], System[StkPlRand() % 6], Subtitle[StkPlRand() % 6]);
				wchar_t DescBuf[30] = L"";
				StkPlSwPrintf(DescBuf, 30, L"%ls %ls %ls", Desc1[StkPlRand() % 5], Desc2[StkPlRand() % 2], Desc3[StkPlRand() % 2]);

				StkObject* Table = new StkObject(L"assignment");
				StkObject* Rec0 = new StkObject(L"RecordInfo", Page * 30 + Loop + 1);
				StkObject* Rec1 = new StkObject(L"RecordInfo", NameBuf);
				StkObject* Rec2 = new StkObject(L"RecordInfo", DescBuf);
				StkObject* Rec3 = new StkObject(L"RecordInfo", (StkPlRand() % 20) * 1000000);
				Table->AppendChildElement(Rec0);
				Table->AppendChildElement(Rec1);
				Table->AppendChildElement(Rec2);
				Table->AppendChildElement(Rec3);
				Root->AppendChildElement(Table);
			}
			if (DbAcc->InsertRecord(Root, StateMsg, Msg) != 0) {
				ShowErrorMsg(StateMsg, Msg);
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
	bool ErrorFlag = false;
	StkPlPrintf("Drop table ... ");
	if (DbAcc->DropTable(L"test_table", StateMsg, Msg) != 0) {
		ShowErrorMsg(StateMsg, Msg);
		ErrorFlag = true;
	}
	if (DbAcc->DropTable(L"𠮷a♩あ🎵z☺", StateMsg, Msg) != 0) {
		ShowErrorMsg(StateMsg, Msg);
		ErrorFlag = true;
	}
	if (DbAcc->DropTable(L"🀅🀅🀅🔥🔥🔥🔥🌈🌈🌈🌈🚀🚀🚀🚀", StateMsg, Msg) != 0) {
		ShowErrorMsg(StateMsg, Msg);
		ErrorFlag = true;
	}
	if (DbAcc->DropTable(L"system_engineer", StateMsg, Msg) != 0) {
		ShowErrorMsg(StateMsg, Msg);
		ErrorFlag = true;
	}
	if (DbAcc->DropTable(L"assignment", StateMsg, Msg) != 0) {
		ShowErrorMsg(StateMsg, Msg);
		ErrorFlag = true;
	}
	if (!ErrorFlag) {
		StkPlPrintf("OK\r\n");
		ErrorFlag = true;
	}

	delete DbAcc;
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
		StkPlPrintf("Usage: tescmd dbms odbc_connection_string test_scenario\n");
		StkPlPrintf("dbms:\n");
		StkPlPrintf("    postgresql, mysql or mariadb\n");
		StkPlPrintf("test_scenario:\n");
		StkPlPrintf("    GENERAL ... 3 tables, 8 columns and 150 records in each table.\n");
		StkPlPrintf("    SE_SERVICE ... DB for Virtual SE Service.\n");
		StkPlPrintf("    CLEANUP ... Drop all tables which were created in this test command.\n");
		StkPlPrintf("\n");
		StkPlPrintf("ex. testcmd postgresql \"Driver={PostgreSQL};Server=127.0.0.1;Database=testdb;UID=admin;PWD=admin;Port=5432;\" GENERAL\n");
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
	} else {
		StkPlPrintf("Error : Unrecognized DBMS specified. [%s]\n", argv[1]);
		StkPlExit(0);
	}

	DbAccessor::Init();
	if (StkPlStrCmp(argv[3], "GENERAL") == 0 && TestGeneral(OdbcConStr, DbmsType) != 0) {
		//
	} else if (StkPlStrCmp(argv[3], "SE_SERVICE") == 0 && TestSeService(OdbcConStr, DbmsType) != 0) {
		//
	}  else if (StkPlStrCmp(argv[3], "CLEANUP") == 0 && TestCleanup(OdbcConStr, DbmsType) != 0) {
		//
	}
	DbAccessor::Uninit();

	delete[] Dbms;
	delete[] OdbcConStr;
	delete[] Scenario;
	return 0;
}
