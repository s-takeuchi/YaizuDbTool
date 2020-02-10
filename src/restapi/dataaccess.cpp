#include "dataaccess.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "..\Global.h"
#include "..\..\..\YaizuComLib\src\stkdata\stkdata.h"
#include "..\..\..\YaizuComLib\src\stkdata\stkdataapi.h"

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

// Add log message
// LogMsgEn [in] : Message in English which you want to insert
// LogMsgJa [in] : Message in Japanese which you want to insert
// Return : always zero returned.
int DataAccess::AddLogMsg(wchar_t LogMsgEn[Global::MAXLEN_OF_LOGMSG], wchar_t LogMsgJa[Global::MAXLEN_OF_LOGMSG])
{
	static int MaxLogId = 0;
	if (MaxLogId == 0) {
		MaxLogId = GetMaxLogId() + 1;
		DeleteOldLogs();
	}
	static int DelFlag = 0;
	if (DelFlag == 10) {
		DeleteOldLogs();
		DelFlag = 0;
	} else {
		DelFlag++;
	}

	wchar_t LocalTimeBuf[64];
	StkPlGetWTimeInIso8601(LocalTimeBuf, true);
	// New record information
	ColumnData *ColDatLog[4];
	ColDatLog[0] = new ColumnDataInt(L"Id", MaxLogId);
	ColDatLog[1] = new ColumnDataWStr(L"Time", LocalTimeBuf);
	ColDatLog[2] = new ColumnDataWStr(L"MessageEn", LogMsgEn);
	ColDatLog[3] = new ColumnDataWStr(L"MessageJa", LogMsgJa);
	RecordData* RecDatLog = new RecordData(L"Log", ColDatLog, 4);
	// Add record
	LockTable(L"Log", LOCK_EXCLUSIVE);
	int Ret = InsertRecord(RecDatLog);
	UnlockTable(L"Log");
	delete RecDatLog;
	MaxLogId++;
	return 0;
}

// Get maximum log id.
// This method checks all of registered log ids and returns the maximum id
// Return : Maximum log id
int DataAccess::GetMaxLogId()
{
	LockTable(L"Log", LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(L"Log");
	UnlockTable(L"Log");

	RecordData* CurrRecDat = RecDatLog;
	int MaxLogId = 0;
	while (CurrRecDat != NULL) {
		ColumnDataInt* ColDat = (ColumnDataInt*)CurrRecDat->GetColumn(0);
		int CurrId = ColDat->GetValue();
		if (CurrId > MaxLogId) {
			MaxLogId = CurrId;
		}
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return MaxLogId;
}

// Get number of logs.
// This method gets number of logs
// Return : Number of logs
int DataAccess::GetNumOfLogs()
{
	LockTable(L"Log", LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(L"Log");
	UnlockTable(L"Log");

	RecordData* CurrRecDat = RecDatLog;
	int NumOfLogs = 0;
	while (CurrRecDat != NULL) {
		NumOfLogs++;
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return NumOfLogs;
}

// Get log information
// LogMsgEn [out] : Acquired log message in English
// LogMsgJa [out] : Acquired log message in Japan
// Return : Number of acquired log messages
int DataAccess::GetLogs(wchar_t LogMsgTime[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGTIME],
						wchar_t LogMsgEn[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG],
						wchar_t LogMsgJa[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG])
{
	LockTable(L"Log", LOCK_EXCLUSIVE);
	AzSortRecord(L"Log", L"Id");
	RecordData* RecDatLog = GetRecord(L"Log");
	UnlockTable(L"Log");

	int NumOfRec = 0;
	RecordData* CurrRecDat = RecDatLog;
	while (CurrRecDat != NULL) {
		ColumnDataWStr* ColDatTime = (ColumnDataWStr*)CurrRecDat->GetColumn(1);
		ColumnDataWStr* ColDatMsgEn = (ColumnDataWStr*)CurrRecDat->GetColumn(2);
		ColumnDataWStr* ColDatMsgJa = (ColumnDataWStr*)CurrRecDat->GetColumn(3);
		if (ColDatTime != NULL && ColDatTime->GetValue() != NULL) {
			StkPlWcsCpy(LogMsgTime[NumOfRec], Global::MAXLEN_OF_LOGTIME, ColDatTime->GetValue());
		} else {
			StkPlWcsCpy(LogMsgTime[NumOfRec], Global::MAXLEN_OF_LOGTIME, L"");
		}
		if (ColDatMsgEn != NULL && ColDatMsgEn->GetValue() != NULL) {
			StkPlWcsCpy(LogMsgEn[NumOfRec], Global::MAXLEN_OF_LOGMSG, ColDatMsgEn->GetValue());
		} else {
			StkPlWcsCpy(LogMsgEn[NumOfRec], Global::MAXLEN_OF_LOGMSG, L"");
		}
		if (ColDatMsgJa != NULL && ColDatMsgJa->GetValue() != NULL) {
			StkPlWcsCpy(LogMsgJa[NumOfRec], Global::MAXLEN_OF_LOGMSG, ColDatMsgJa->GetValue());
		} else {
			StkPlWcsCpy(LogMsgJa[NumOfRec], Global::MAXLEN_OF_LOGMSG, L"");
		}
		NumOfRec++;
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return NumOfRec;
}

// This function deletes old logs.
// Return : Always zero returned
int DataAccess::DeleteOldLogs()
{
	int NumOfLogs = GetNumOfLogs();
	if (NumOfLogs >= 100) {
		LockTable(L"Log", LOCK_EXCLUSIVE);
		AzSortRecord(L"Log", L"Id");
		RecordData* RecDatLog = GetRecord(L"Log");
		UnlockTable(L"Log");
		int ExceededNumOfLogs = NumOfLogs - 99;
		RecordData* CurrRecDat = RecDatLog;
		for (int Loop = 0; Loop < ExceededNumOfLogs; Loop++) {
			ColumnDataInt* ColDatId = (ColumnDataInt*)CurrRecDat->GetColumn(0);
			int ValueId = ColDatId->GetValue();

			ColumnData* DelColDat[1];
			DelColDat[0] = new ColumnDataInt(L"Id", ValueId);
			RecordData* DelRecDat = new RecordData(L"Log", DelColDat, 1);
			LockTable(L"Log", LOCK_EXCLUSIVE);
			DeleteRecord(DelRecDat);
			UnlockTable(L"Log");
			delete DelRecDat;

			CurrRecDat = CurrRecDat->GetNextRecord();
		}
		delete RecDatLog;
	}

	return 0;
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
			StkPlWcsCpy(Name[Loop], Global::MAXLEN_OF_PASSWORD, ColDatName->GetValue());
			StkPlWcsCpy(Password[Loop], Global::MAXLEN_OF_PASSWORD, ColDatPw->GetValue());
			Role[Loop] = ColDatRole->GetValue();
		}
		CurRecDatUser = CurRecDatUser->GetNextRecord();
	}
	delete RecDatUser;
	return Loop;
}

bool DataAccess::UpdateUser(int Id, wchar_t Name[Global::MAXLEN_OF_USERNAME], int Role)
{
	ColumnData* ColDatUpdUser[3];
	ColDatUpdUser[0] = new ColumnDataInt(L"Id", Id);
	ColDatUpdUser[1] = new ColumnDataWStr(L"Name", Name);
	ColDatUpdUser[2] = new ColumnDataInt(L"Role", Role);
	RecordData* RecDatUpdUser = new RecordData(L"User", ColDatUpdUser, 3);

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
	AutoSave(Buf, 30, true);

	LockAllTable(2);
	if (StkPlGetFileSize(DataFileName) == 0) {

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

		// Log table
		ColumnDefInt ColDefLogId(L"Id");
		ColumnDefWStr ColDefLogTime(L"Time", Global::MAXLEN_OF_LOGTIME);
		ColumnDefWStr ColDefLogMsgEn(L"MessageEn", Global::MAXLEN_OF_LOGMSG);
		ColumnDefWStr ColDefLogMsgJa(L"MessageJa", Global::MAXLEN_OF_LOGMSG);
		TableDef TabDefLog(L"Log", Global::MAXNUM_OF_LOGRECORDS);
		TabDefLog.AddColumnDef(&ColDefLogId);
		TabDefLog.AddColumnDef(&ColDefLogTime);
		TabDefLog.AddColumnDef(&ColDefLogMsgEn);
		TabDefLog.AddColumnDef(&ColDefLogMsgJa);
		if (CreateTable(&TabDefLog) != 0) {
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

		// User table
		ColumnDefInt ColDefUserId(L"Id");
		ColumnDefWStr ColDefUserName(L"Name", Global::MAXLEN_OF_USERNAME);
		ColumnDefWStr ColDefUserPassword(L"Password", Global::MAXLEN_OF_PASSWORD);
		ColumnDefInt ColDefUserRole(L"Role");
		TableDef TabDefUser(L"User", Global::MAXNUM_OF_USERRECORDS);
		TabDefUser.AddColumnDef(&ColDefUserId);
		TabDefUser.AddColumnDef(&ColDefUserName);
		TabDefUser.AddColumnDef(&ColDefUserPassword);
		TabDefUser.AddColumnDef(&ColDefUserRole);
		if (CreateTable(&TabDefUser) != 0) {
			UnlockAllTable();
			return -1;
		}

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

		// Add records for User
		{
			ColumnData *ColDatUser[4];
			ColDatUser[0] = new ColumnDataInt(L"Id", 0);
			ColDatUser[1] = new ColumnDataWStr(L"Name", L"admin@a.a");
			ColDatUser[2] = new ColumnDataWStr(L"Password", L"manager");
			ColDatUser[3] = new ColumnDataInt(L"Role", 0);
			RecordData* RecUser = new RecordData(L"User", ColDatUser, 4);
			// Add record
			LockTable(L"User", LOCK_EXCLUSIVE);
			int Ret = InsertRecord(RecUser);
			UnlockTable(L"User");
			delete RecUser;
		}
		{
			ColumnData *ColDatUser[4];
			ColDatUser[0] = new ColumnDataInt(L"Id", 1);
			ColDatUser[1] = new ColumnDataWStr(L"Name", L"takeuchi@a.a");
			ColDatUser[2] = new ColumnDataWStr(L"Password", L"takeuchi");
			ColDatUser[3] = new ColumnDataInt(L"Role", 1);
			RecordData* RecUser = new RecordData(L"User", ColDatUser, 4);
			// Add record
			LockTable(L"User", LOCK_EXCLUSIVE);
			int Ret = InsertRecord(RecUser);
			UnlockTable(L"User");
			delete RecUser;
		}

	} else {
		if (LoadData(Buf) != 0) {
			UnlockAllTable();
			return -1;
		}
	}
	UnlockAllTable();

	return 0;
}
