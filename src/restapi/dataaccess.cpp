#include "dataaccess.h"
#include "Global.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkdata/stkdata.h"
#include "../../../YaizuComLib/src/stkdata/stkdataapi.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"

DataAccess* DataAccess::ThisInstance;

DataAccess::DataAccess()
{
	StkPlWcsCpy(DataFileName, FILENAME_MAX, L"sample.dat");
}

DataAccess::~DataAccess()
{
}

// Get this instance
DataAccess* DataAccess::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new DataAccess();
		Init = 0;
	}
	return ThisInstance;
}

// Configure ODBC connection string
// ConnStr [in] : Connection string
void DataAccess::SetOdbcConnStr(int DbmsType, wchar_t ConnStr[256])
{
	// Record for update
	ColumnData *ColDatUpd[3];
	ColDatUpd[0] = new ColumnDataWStr(L"ConnStr", ConnStr);
	ColDatUpd[1] = new ColumnDataInt(L"DbmsType", DbmsType);
	ColDatUpd[2] = new ColumnDataInt(L"Init", 0);
	RecordData* RecDatUpd = new RecordData(L"OdbcConfig", ColDatUpd, 3);

	// Record for search
	ColumnData *ColDatSch[1];
	ColDatSch[0] = new ColumnDataInt(L"OdbcId", 0);
	RecordData* RecDatSch = new RecordData(L"OdbcConfig", ColDatSch, 1);

	// Add record
	LockTable(L"OdbcConfig", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSch, RecDatUpd);
	UnlockTable(L"OdbcConfig");
	delete RecDatSch;
	delete RecDatUpd;
	return;
}

// Acuqire ODBC configuration
// ConnStr [out] : Acquired connection string
// Init [out] : Init flag (1:After installation, 0:After connection string is configured)
// Return : Type of DBMS (0:MariaDB, 1:PostgreSQL, 2:MySQL, -1:Connection string does not exist)
int DataAccess::GetOdbcConfing(wchar_t ConnStr[256], int* Init)
{
	LockTable(L"OdbcConfig", LOCK_SHARE);
	RecordData* RecDatOdbcConfig = GetRecord(L"OdbcConfig");
	UnlockTable(L"OdbcConfig");
	ColumnDataInt* ColDatOdbcId = (ColumnDataInt*)RecDatOdbcConfig->GetColumn(0);
	ColumnDataInt* ColDatDbmsType = (ColumnDataInt*)RecDatOdbcConfig->GetColumn(1);
	ColumnDataWStr* ColDatConnStr = (ColumnDataWStr*)RecDatOdbcConfig->GetColumn(2);
	ColumnDataInt* ColDatInit = (ColumnDataInt*)RecDatOdbcConfig->GetColumn(3);
	if (ColDatOdbcId == NULL || ColDatDbmsType == NULL || ColDatConnStr == NULL || ColDatInit == NULL) {
		return -1;
	}

	int OdbcId = ColDatOdbcId->GetValue();
	int DbmsType = ColDatDbmsType->GetValue();
	StkPlWcsCpy(ConnStr, 256, ColDatConnStr->GetValue());
	*Init = ColDatInit->GetValue();
	delete RecDatOdbcConfig;
	return DbmsType;
}

// Set filter conditions
// Index [in] : each number of 1 to 5
// ColumnName [in] : Column name which filter condition
// FilterOpeType[in] : Comparison type
// Value [in] : Value which filter condition
void DataAccess::SetFilterCondition(int Index, wchar_t ColumnName[Global::COLUMNNAME_LENGTH], int FilterOpeType, wchar_t Value[Global::COLUMNVAL_LENGTH])
{
	ColumnData *ColDatFilter[4];
	ColDatFilter[0] = new ColumnDataInt(L"Index", Index);
	ColDatFilter[1] = new ColumnDataWStr(L"Column", ColumnName);
	ColDatFilter[2] = new ColumnDataInt(L"Operation", FilterOpeType);
	ColDatFilter[3] = new ColumnDataWStr(L"Value", Value);
	RecordData* RecDatFilter = new RecordData(L"Filter", ColDatFilter, 4);

	ColumnData *ColDatSearch[1];
	ColDatSearch[0] = new ColumnDataInt(L"Index", Index);
	RecordData* RecDatSearch = new RecordData(L"Filter", ColDatSearch, 1);

	LockTable(L"Filter", LOCK_EXCLUSIVE);
	UpdateRecord(RecDatSearch, RecDatFilter);
	UnlockTable(L"Filter");
	delete RecDatSearch;
	delete RecDatFilter;
}

// Get filter conditions
// Index [in] : each number of 1 to 5
// ColumnName [out] : Column name which filter condition
// FilterOpeType[out] : pointer to comparison type
// Value [out] : Value which filter condition
void DataAccess::GetFilterCondition(int Index, wchar_t ColumnName[Global::COLUMNNAME_LENGTH], int* FilterOpeType, wchar_t Value[Global::COLUMNVAL_LENGTH])
{
	LockTable(L"Filter", LOCK_SHARE);
	RecordData* RecDatFilter = GetRecord(L"Filter");
	UnlockTable(L"Filter");
	RecordData* CurRecDat = RecDatFilter;
	while (CurRecDat) {
		ColumnDataInt* ColDatIndex = (ColumnDataInt*)CurRecDat->GetColumn(0);
		ColumnDataWStr* ColDatColumnName = (ColumnDataWStr*)CurRecDat->GetColumn(1);
		ColumnDataInt* ColDatOpeType = (ColumnDataInt*)CurRecDat->GetColumn(2);
		ColumnDataWStr* ColDatValue = (ColumnDataWStr*)CurRecDat->GetColumn(3);
		if (Index == ColDatIndex->GetValue()) {
			StkPlWcsCpy(ColumnName, Global::COLUMNNAME_LENGTH, ColDatColumnName->GetValue());
			*FilterOpeType = ColDatOpeType->GetValue();
			StkPlWcsCpy(Value, Global::COLUMNVAL_LENGTH, ColDatValue->GetValue());
			break;
		}
		CurRecDat = CurRecDat->GetNextRecord();
	}
	delete RecDatFilter;
}

// Set filtering switch
// Switch [in] : TRUE:On, FALSE:Off
void DataAccess::SetFilterSwitch(bool Switch)
{
	// Record for update
	ColumnData *ColDatUpd[1];
	ColDatUpd[0] = new ColumnDataInt(L"Switch", (Switch == true)? 1:0 );
	RecordData* RecDatUpd = new RecordData(L"FilterSw", ColDatUpd, 1);

	// Record for search
	ColumnData *ColDatSch[1];
	ColDatSch[0] = new ColumnDataInt(L"Switch", (Switch == true)? 0:1 );
	RecordData* RecDatSch = new RecordData(L"FilterSw", ColDatSch, 1);

	// Add record
	LockTable(L"FilterSw", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSch, RecDatUpd);
	UnlockTable(L"FilterSw");
	delete RecDatSch;
	delete RecDatUpd;
	return;
}

// Get filtering switch
// return : TRUE:On, FALSE:Off
bool DataAccess::GetFilterSwitch()
{
	LockTable(L"FilterSw", LOCK_SHARE);
	RecordData* RecDatFilterSw = GetRecord(L"FilterSw");
	UnlockTable(L"FilterSw");

	ColumnDataInt* ColDatSwitch = (ColumnDataInt*)RecDatFilterSw->GetColumn(0);
	if (ColDatSwitch == NULL) {
		return false;
	}
	int Switch = ColDatSwitch->GetValue();
	delete RecDatFilterSw;
	return (Switch == 1)? true : false;
}

bool DataAccess::GetTargetUserByName(wchar_t Name[Global::MAXLEN_OF_USERNAME], int* Id, wchar_t Password[Global::MAXLEN_OF_PASSWORD], int* Role)
{
	ColumnData* ColDat[1];
	ColDat[0] = new ColumnDataWStr(L"Name", Name);
	RecordData* SearchUser = new RecordData(L"User", ColDat, 1);
	LockTable(L"User", LOCK_SHARE);
	RecordData* RecDatUser = GetRecord(SearchUser);
	UnlockTable(L"User");
	delete SearchUser;
	if (RecDatUser != NULL) {
		ColumnDataInt* ColDatId = (ColumnDataInt*)RecDatUser->GetColumn(0);
		ColumnDataWStr* ColDatPw = (ColumnDataWStr*)RecDatUser->GetColumn(2);
		ColumnDataInt* ColDatRole = (ColumnDataInt*)RecDatUser->GetColumn(3);
		if (ColDatId != NULL && ColDatPw != NULL && ColDatRole != NULL) {
			*Id = ColDatId->GetValue();
			StkPlWcsCpy(Password, Global::MAXLEN_OF_PASSWORD, ColDatPw->GetValue());
			*Role = ColDatRole->GetValue();
		}
	} else {
		return false;
	}
	delete RecDatUser;
	return true;
}

bool DataAccess::GetTargetUserById(int Id, wchar_t Name[Global::MAXLEN_OF_USERNAME], wchar_t Password[Global::MAXLEN_OF_PASSWORD], int* Role)
{
	ColumnData* ColDat[1];
	ColDat[0] = new ColumnDataInt(L"Id", Id);
	RecordData* SearchUser = new RecordData(L"User", ColDat, 1);
	LockTable(L"User", LOCK_SHARE);
	RecordData* RecDatUser = GetRecord(SearchUser);
	UnlockTable(L"User");
	delete SearchUser;
	if (RecDatUser != NULL) {
		ColumnDataWStr* ColDatName = (ColumnDataWStr*)RecDatUser->GetColumn(1);
		ColumnDataWStr* ColDatPw = (ColumnDataWStr*)RecDatUser->GetColumn(2);
		ColumnDataInt* ColDatRole = (ColumnDataInt*)RecDatUser->GetColumn(3);
		if (ColDatName != NULL && ColDatPw != NULL && ColDatRole != NULL) {
			StkPlWcsCpy(Name, Global::MAXLEN_OF_USERNAME, ColDatName->GetValue());
			StkPlWcsCpy(Password, Global::MAXLEN_OF_PASSWORD, ColDatPw->GetValue());
			*Role = ColDatRole->GetValue();
		}
	} else {
		return false;
	}
	delete RecDatUser;
	return true;
}

int DataAccess::GetTargetUsers(int Id[Global::MAXNUM_OF_USERRECORDS],
	wchar_t Name[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_USERNAME],
	wchar_t Password[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_PASSWORD],
	int Role[Global::MAXNUM_OF_USERRECORDS])
{
	LockTable(L"User", LOCK_SHARE);
	RecordData* RecDatUser = GetRecord(L"User");
	UnlockTable(L"User");
	RecordData* CurRecDatUser = RecDatUser;
	if (!CurRecDatUser) {
		return 0;
	}
	int Loop = 0;
	for (; CurRecDatUser; Loop++) {
		ColumnDataInt* ColDatId = (ColumnDataInt*)CurRecDatUser->GetColumn(0);
		ColumnDataWStr* ColDatName = (ColumnDataWStr*)CurRecDatUser->GetColumn(1);
		ColumnDataWStr* ColDatPw = (ColumnDataWStr*)CurRecDatUser->GetColumn(2);
		ColumnDataInt* ColDatRole = (ColumnDataInt*)CurRecDatUser->GetColumn(3);
		if (ColDatId != NULL && ColDatName != NULL && ColDatPw != NULL && ColDatRole != NULL) {
			Id[Loop] = ColDatId->GetValue();
			StkPlWcsCpy(Name[Loop], Global::MAXLEN_OF_USERNAME, ColDatName->GetValue());
			StkPlWcsCpy(Password[Loop], Global::MAXLEN_OF_PASSWORD, ColDatPw->GetValue());
			Role[Loop] = ColDatRole->GetValue();
		}
		CurRecDatUser = CurRecDatUser->GetNextRecord();
	}
	delete RecDatUser;
	return Loop;
}

int DataAccess::GetNumberOfUsers()
{
	return GetNumOfRecords(L"User");
}

bool DataAccess::AddUser(wchar_t Name[Global::MAXLEN_OF_USERNAME], int Role, wchar_t Password[Global::MAXLEN_OF_PASSWORD])
{
	LockTable(L"User", LOCK_EXCLUSIVE);

	// Get max user ID
	RecordData* RecDatUser = GetRecord(L"User");
	RecordData* CurrRecDat = RecDatUser;
	int MaxLogId = 0;
	while (CurrRecDat != NULL) {
		ColumnDataInt* ColDat = (ColumnDataInt*)CurrRecDat->GetColumn(0);
		int CurrId = ColDat->GetValue();
		if (CurrId > MaxLogId) {
			MaxLogId = CurrId;
		}
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatUser;
	
	// Add new user
	ColumnData* ColDatInsertUser[4];
	ColDatInsertUser[0] = new ColumnDataInt(L"Id", MaxLogId + 1);
	ColDatInsertUser[1] = new ColumnDataWStr(L"Name", Name);
	ColDatInsertUser[2] = new ColumnDataWStr(L"Password", Password);
	ColDatInsertUser[3] = new ColumnDataInt(L"Role", Role);
	RecordData* RecDatInsertUser = new RecordData(L"User", ColDatInsertUser, 4);
	int Ret = InsertRecord(RecDatInsertUser);
	delete RecDatInsertUser;

	UnlockTable(L"User");
	return true;
}

bool DataAccess::UpdateUser(int Id, wchar_t Name[Global::MAXLEN_OF_USERNAME], int Role, wchar_t Password[Global::MAXLEN_OF_PASSWORD])
{
	int ColLen = 0;
	ColumnData* ColDatUpdUser[4];
	ColDatUpdUser[0] = new ColumnDataInt(L"Id", Id);
	if (StkPlWcsCmp(Name, L"") != 0) {
		ColLen++;
		ColDatUpdUser[ColLen] = new ColumnDataWStr(L"Name", Name);
	}
	if (Role != -1) {
		ColLen++;
		ColDatUpdUser[ColLen] = new ColumnDataInt(L"Role", Role);
	}
	if (Password != NULL && Password[0] != L'\0') {
		ColLen++;
		ColDatUpdUser[ColLen] = new ColumnDataWStr(L"Password", Password);
	}
	RecordData* RecDatUpdUser = new RecordData(L"User", ColDatUpdUser, ColLen + 1);

	ColumnData* ColDatSearchUser[1];
	ColDatSearchUser[0] = new ColumnDataInt(L"Id", Id);
	RecordData* RecDatSearchUser = new RecordData(L"User", ColDatSearchUser, 1);

	LockTable(L"User", LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSearchUser, RecDatUpdUser);
	UnlockTable(L"User");
	delete RecDatUpdUser;
	delete RecDatSearchUser;
	return true;
}

bool DataAccess::DeleteUser(int Id)
{
	ColumnData* ColDatSearchUser[1];
	ColDatSearchUser[0] = new ColumnDataInt(L"Id", Id);
	RecordData* RecDatSearchUser = new RecordData(L"User", ColDatSearchUser, 1);

	LockTable(L"User", LOCK_EXCLUSIVE);
	int Ret = DeleteRecord(RecDatSearchUser);
	UnlockTable(L"User");
	delete RecDatSearchUser;
	return true;
}

// Get DB version
int DataAccess::GetDbVersion()
{
	// Check whether "Property" table exists
	int RetLk = LockTable(L"Property", LOCK_SHARE);
	if (RetLk == -1) {
		return -1;
	}
	UnlockTable(L"Property");
	return StkWebAppUm_GetPropertyValueInt(L"DbVersion");
}

int DataAccess::DbUpdate_NonVer_V3()
{
	{
		// Create Property table
		ColumnDefWStr ColDefPropertyName(L"Name", 256);
		ColumnDefInt ColDefPropertyValueInt(L"ValueInt");
		ColumnDefWStr ColDefPropertyValueWStr(L"ValueWStr", 256);
		TableDef TabDefProperty(L"Property", 1024);
		TabDefProperty.AddColumnDef(&ColDefPropertyName);
		TabDefProperty.AddColumnDef(&ColDefPropertyValueInt);
		TabDefProperty.AddColumnDef(&ColDefPropertyValueWStr);
		if (CreateTable(&TabDefProperty) != 0) {
			UnlockAllTable();
			return -1;
		}
	}

	{
		//Property setting

		// Get maximum user ID
		LockTable(L"User", LOCK_SHARE);
		RecordData* RecDatUser = GetRecord(L"User");
		UnlockTable(L"User");
		int MaxUserId = 0;
		while (RecDatUser) {
			ColumnDataInt* ColDatId = (ColumnDataInt*)RecDatUser->GetColumn(0);
			int CurUserId = ColDatId->GetValue();
			if (CurUserId > MaxUserId) {
				MaxUserId = CurUserId;
			}
			RecDatUser = RecDatUser->GetNextRecord();
		}
		delete RecDatUser;

		// Get maximum log ID
		LockTable(L"Log", LOCK_SHARE);
		RecordData* RecDatLog = GetRecord(L"Log");
		UnlockTable(L"Log");
		int MaxLogId = 0;
		while (RecDatLog) {
			ColumnDataInt* ColDatId = (ColumnDataInt*)RecDatLog->GetColumn(0);
			int CurLogId = ColDatId->GetValue();
			if (CurLogId > MaxLogId) {
				MaxLogId = CurLogId;
			}
			RecDatLog = RecDatLog->GetNextRecord();
		}
		delete RecDatLog;

		// Set Property values
		StkWebAppUm_SetPropertyValueInt(L"MaxUserId", MaxUserId);
		StkWebAppUm_SetPropertyValueInt(L"MaxLogId", MaxLogId);
		StkWebAppUm_SetPropertyValueInt(L"DbVersion", LatestDbVersion);
	}

	{
		// Log migration
		
		// Log table migration (Step.1 Create new table)
		ColumnDefInt ColDefLogId(L"Id");
		ColumnDefWStr ColDefLogTime(L"Time", Global::MAXLEN_OF_LOGTIME);
		ColumnDefInt ColDefLogUserId(L"UserId");
		ColumnDefWStr ColDefLogMsgEn(L"MessageEn", Global::MAXLEN_OF_LOGMSG);
		ColumnDefWStr ColDefLogMsgJa(L"MessageJa", Global::MAXLEN_OF_LOGMSG);
		TableDef TabDefLog(L"LogNew", Global::MAXNUM_OF_LOGRECORDS);
		TabDefLog.AddColumnDef(&ColDefLogId);
		TabDefLog.AddColumnDef(&ColDefLogTime);
		TabDefLog.AddColumnDef(&ColDefLogUserId);
		TabDefLog.AddColumnDef(&ColDefLogMsgEn);
		TabDefLog.AddColumnDef(&ColDefLogMsgJa);
		if (CreateTable(&TabDefLog) != 0) {
			UnlockAllTable();
			return -1;
		}
		// Log table migration (Step.2 data acquisition)
		LockTable(L"Log", LOCK_EXCLUSIVE);
		RecordData* RecDatLog = GetRecord(L"Log");
		UnlockTable(L"Log");
		// Log table migration (Step.3 data migration)
		LockTable(L"LogNew", LOCK_EXCLUSIVE);
		RecordData* CurrRecDat = RecDatLog;
		while (CurrRecDat != NULL) {
			ColumnDataInt* ColDatId = (ColumnDataInt*)CurrRecDat->GetColumn(0);
			ColumnDataWStr* ColDatTime = (ColumnDataWStr*)CurrRecDat->GetColumn(1);
			ColumnDataWStr* ColDatMsgEn = (ColumnDataWStr*)CurrRecDat->GetColumn(2);
			ColumnDataWStr* ColDatMsgJa = (ColumnDataWStr*)CurrRecDat->GetColumn(3);
			if (ColDatId != NULL &&
				ColDatTime != NULL && ColDatTime->GetValue() != NULL &&
				ColDatMsgEn != NULL && ColDatMsgEn->GetValue() != NULL &&
				ColDatMsgJa != NULL && ColDatMsgJa->GetValue() != NULL) {

				// Time conversion
				long long LocalTime = StkPlGetUnixTimeFromIso8601W(ColDatTime->GetValue());
				wchar_t LocalTimeInStr[64] = L"";
				StkPlSwPrintf(LocalTimeInStr, 64, L"%016x", LocalTime);

				// New record information
				ColumnData* ColDatLog[5];
				ColDatLog[0] = new ColumnDataInt(L"Id", ColDatId->GetValue());
				ColDatLog[1] = new ColumnDataWStr(L"Time", LocalTimeInStr);
				ColDatLog[2] = new ColumnDataInt(L"UserId", -2);
				ColDatLog[3] = new ColumnDataWStr(L"MessageEn", ColDatMsgEn->GetValue());
				ColDatLog[4] = new ColumnDataWStr(L"MessageJa", ColDatMsgJa->GetValue());
				RecordData* RecDatLog = new RecordData(L"LogNew", ColDatLog, 5);
				// Add record
				int Ret = InsertRecord(RecDatLog);
				delete RecDatLog;
			}
			CurrRecDat = CurrRecDat->GetNextRecord();
		}
		delete RecDatLog;
		UnlockTable(L"LogNew");
		// Log table migration (Step.4 delete old table)
		DeleteTable(L"Log");
		RenameTable(L"LogNew", L"Log");
	}

	return 3;
}

// Stops AutoSave function and save the latest data
// Return : always zero returned
int DataAccess::StopAutoSave()
{
	wchar_t Buf[FILENAME_MAX];
	StkPlGetFullPathFromFileName(DataFileName, Buf);
	AutoSave(Buf, 30, false);
	LockAllTable(2);
	SaveData(Buf);
	UnlockAllTable();
	return 0;
}

// Create tables for CmdFreak
// Return : [0:Success, -1:Failed]
int DataAccess::CreateCmdFreakTables()
{
	// Make full path name and call AutoSave
	wchar_t Buf[FILENAME_MAX];
	StkPlGetFullPathFromFileName(DataFileName, Buf);
	size_t WorkDatLength = StkPlGetFileSize(Buf);
	wchar_t LogBuf[1024] = L"";
	if (WorkDatLength == (size_t)-1) {
		StkPlSwPrintf(LogBuf, 1024, L"The data file was not found at [%ls]", Buf);
		MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

#ifdef WIN32
		return -1;
#endif
#ifndef WIN32
		StkPlSwPrintf(Buf, FILENAME_MAX, L"/etc/%ls", DataFileName);
#endif
	}
	StkPlWcsCpy(DataFileName, FILENAME_MAX, Buf);
	StkPlSwPrintf(LogBuf, 1024, L"The data file path = [%ls]", Buf);
	MessageProc::AddLog(LogBuf, MessageProc::LOG_TYPE_INFO);

	LockAllTable(2);
	if (StkPlGetFileSize(Buf) == 0) {
		MessageProc::AddLog("Execute database initialization", MessageProc::LOG_TYPE_INFO);

		// OdbcConfig table
		ColumnDefInt ColDefOdbcId(L"OdbcId");
		ColumnDefInt ColDefDbmsType(L"DbmsType");
		ColumnDefWStr ColDefConnStr(L"ConnStr", 256);
		ColumnDefInt ColDefOdbcInit(L"Init");
		TableDef TabDefOdbcConfig(L"OdbcConfig", 50);
		TabDefOdbcConfig.AddColumnDef(&ColDefOdbcId);
		TabDefOdbcConfig.AddColumnDef(&ColDefDbmsType);
		TabDefOdbcConfig.AddColumnDef(&ColDefConnStr);
		TabDefOdbcConfig.AddColumnDef(&ColDefOdbcInit);
		if (CreateTable(&TabDefOdbcConfig) != 0) {
			UnlockAllTable();
			return -1;
		}

		// FilterSw table
		ColumnDefInt ColDefSwitch(L"Switch");
		TableDef TabDefFilterSw(L"FilterSw", 5);
		TabDefFilterSw.AddColumnDef(&ColDefSwitch);
		if (CreateTable(&TabDefFilterSw) != 0) {
			UnlockAllTable();
			return -1;
		}

		// Filter table
		ColumnDefInt ColDefIndex(L"Index");
		ColumnDefWStr ColDefColumn(L"Column", Global::COLUMNNAME_LENGTH);
		ColumnDefInt ColDefOperation(L"Operation");
		ColumnDefWStr ColDefValue(L"Value", Global::COLUMNVAL_LENGTH);
		TableDef TabDefFilter(L"Filter", 10);
		TabDefFilter.AddColumnDef(&ColDefIndex);
		TabDefFilter.AddColumnDef(&ColDefColumn);
		TabDefFilter.AddColumnDef(&ColDefOperation);
		TabDefFilter.AddColumnDef(&ColDefValue);
		if (CreateTable(&TabDefFilter) != 0) {
			UnlockAllTable();
			return -1;
		}
		UnlockAllTable();

		int Ret;

		// Add record for OdbcConfig
		ColumnData *ColDatOdbcConfig[4];
		ColDatOdbcConfig[0] = new ColumnDataInt(L"OdbcId", 0);
		ColDatOdbcConfig[1] = new ColumnDataInt(L"DbmsType", 0);
		ColDatOdbcConfig[2] = new ColumnDataWStr(L"ConnStr", L"");
		ColDatOdbcConfig[3] = new ColumnDataInt(L"Init", 1);
		RecordData* RecDatOdbcConfig = new RecordData(L"OdbcConfig", ColDatOdbcConfig, 4);
		LockTable(L"OdbcConfig", LOCK_EXCLUSIVE);
		Ret = InsertRecord(RecDatOdbcConfig);
		UnlockTable(L"OdbcConfig");
		delete RecDatOdbcConfig;

		// Add record for FilterSw
		ColumnData *ColDatFilterSw[1];
		ColDatFilterSw[0] = new ColumnDataInt(L"Switch", 0);
		RecordData* RecDatFilterSw = new RecordData(L"FilterSw", ColDatFilterSw, 1);
		LockTable(L"FilterSw", LOCK_EXCLUSIVE);
		Ret = InsertRecord(RecDatFilterSw);
		UnlockTable(L"FilterSw");
		delete RecDatFilterSw;

		// Add records for Filter
		for (int Loop = 1; Loop <= 5; Loop++) {
			ColumnData *ColDatFilter[4];
			ColDatFilter[0] = new ColumnDataInt(L"Index", Loop);
			ColDatFilter[1] = new ColumnDataWStr(L"Column", L"*");
			ColDatFilter[2] = new ColumnDataInt(L"Operation", 0);
			ColDatFilter[3] = new ColumnDataWStr(L"Value", L"");
			RecordData* RecDatFilter = new RecordData(L"Filter", ColDatFilter, 4);
			LockTable(L"Filter", LOCK_EXCLUSIVE);
			Ret = InsertRecord(RecDatFilter);
			UnlockTable(L"Filter");
			delete RecDatFilter;
		}

		StkWebAppUm_CreateTable();
		StkWebAppUm_SetPropertyValueInt(L"DbVersion", LatestDbVersion);
		AutoSave(Buf, 30, true);
		return 0;
	} else {
		if (LoadData(Buf) != 0) {
			UnlockAllTable();
			return -1;
		}
		UnlockAllTable();
		AutoSave(Buf, 30, true);
		return 0;
	}
}
