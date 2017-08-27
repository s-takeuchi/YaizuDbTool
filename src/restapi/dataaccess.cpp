#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include "dataaccess.h"
#include "..\Global.h"
#include "..\..\..\YaizuComLib\src\stkdata\stkdata.h"
#include "..\..\..\YaizuComLib\src\stkdata\stkdataapi.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkGeneric.h"

DataAccess* DataAccess::ThisInstance;

DataAccess::DataAccess()
{
	lstrcpyn(DataFileName, _T("stkwebapp.dat"), MAX_PATH);
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
int DataAccess::AddLogMsg(TCHAR LogMsgEn[Global::MAXLEN_OF_LOGMSG], TCHAR LogMsgJa[Global::MAXLEN_OF_LOGMSG])
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

	TCHAR LocalTimeBuf[32];
	StkGeneric::GetInstance()->GetLocalTimeWStr(LocalTimeBuf);
	// New record information
	ColumnData *ColDatLog[4];
	ColDatLog[0] = new ColumnDataInt(_T("Id"), MaxLogId);
	ColDatLog[1] = new ColumnDataWStr(_T("Time"), LocalTimeBuf);
	ColDatLog[2] = new ColumnDataWStr(_T("MessageEn"), LogMsgEn);
	ColDatLog[3] = new ColumnDataWStr(_T("MessageJa"), LogMsgJa);
	RecordData* RecDatLog = new RecordData(_T("Log"), ColDatLog, 4);
	// Add record
	LockTable(_T("Log"), LOCK_EXCLUSIVE);
	int Ret = InsertRecord(RecDatLog);
	UnlockTable(_T("Log"));
	delete RecDatLog;
	MaxLogId++;
	return 0;
}

// Get maximum log id.
// This method checks all of registered log ids and returns the maximum id
// Return : Maximum log id
int DataAccess::GetMaxLogId()
{
	LockTable(_T("Log"), LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(_T("Log"));
	UnlockTable(_T("Log"));

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
	LockTable(_T("Log"), LOCK_SHARE);
	RecordData* RecDatLog = GetRecord(_T("Log"));
	UnlockTable(_T("Log"));

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
int DataAccess::GetLogs(TCHAR LogMsgTime[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGTIME],
						TCHAR LogMsgEn[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG],
						TCHAR LogMsgJa[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG])
{
	LockTable(_T("Log"), LOCK_EXCLUSIVE);
	AzSortRecord(_T("Log"), _T("Id"));
	RecordData* RecDatLog = GetRecord(_T("Log"));
	UnlockTable(_T("Log"));

	int NumOfRec = 0;
	RecordData* CurrRecDat = RecDatLog;
	while (CurrRecDat != NULL) {
		ColumnDataWStr* ColDatTime = (ColumnDataWStr*)CurrRecDat->GetColumn(1);
		ColumnDataWStr* ColDatMsgEn = (ColumnDataWStr*)CurrRecDat->GetColumn(2);
		ColumnDataWStr* ColDatMsgJa = (ColumnDataWStr*)CurrRecDat->GetColumn(3);
		if (ColDatTime != NULL && ColDatTime->GetValue() != NULL) {
			lstrcpy(LogMsgTime[NumOfRec], ColDatTime->GetValue());
		} else {
			lstrcpy(LogMsgTime[NumOfRec], _T(""));
		}
		if (ColDatMsgEn != NULL && ColDatMsgEn->GetValue() != NULL) {
			lstrcpy(LogMsgEn[NumOfRec], ColDatMsgEn->GetValue());
		} else {
			lstrcpy(LogMsgEn[NumOfRec], _T(""));
		}
		if (ColDatMsgJa != NULL && ColDatMsgJa->GetValue() != NULL) {
			lstrcpy(LogMsgJa[NumOfRec], ColDatMsgJa->GetValue());
		} else {
			lstrcpy(LogMsgJa[NumOfRec], _T(""));
		}
		NumOfRec++;
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	delete RecDatLog;
	return NumOfRec;
}

// Get log information as HTML
// LogAsHtml [out] : Acquired log information
// Return : 0:Success, -1: Failure
int DataAccess::GetLogAsHtml(TCHAR LogAsHtml[Global::MAX_PARAM_LENGTH / 2])
{
	LockTable(_T("Log"), LOCK_EXCLUSIVE);
	AzSortRecord(_T("Log"), _T("Id"));
	RecordData* RecDatLog = GetRecord(_T("Log"));
	UnlockTable(_T("Log"));

	StrCpy(LogAsHtml, _T("<table class='tblstyle'>"));
	TCHAR TmpBuf[200];
	wsprintf(TmpBuf, _T("<tr><th>Time</th><th>Message</th></tr>"));
	StrCat(LogAsHtml, TmpBuf);
	RecordData* CurrRecDat = RecDatLog;
	while (CurrRecDat != NULL) {
		ColumnDataWStr* ColDatTime = (ColumnDataWStr*)CurrRecDat->GetColumn(1);
		ColumnDataWStr* ColDatMsg = (ColumnDataWStr*)CurrRecDat->GetColumn(2);
		StrCat(LogAsHtml, _T("<tr><td>"));
		if (ColDatTime->GetValue() != NULL) {
			StrCat(LogAsHtml, ColDatTime->GetValue());
		}
		StrCat(LogAsHtml, _T("</td><td>"));
		if (ColDatMsg->GetValue() != NULL) {
			StrCat(LogAsHtml, ColDatMsg->GetValue());
		}
		StrCat(LogAsHtml, _T("</td></tr>"));
		CurrRecDat = CurrRecDat->GetNextRecord();
	}
	StrCat(LogAsHtml, _T("</table>"));
	delete RecDatLog;
	return 0;
}

// This function deletes old logs.
// Return : Always zero returned
int DataAccess::DeleteOldLogs()
{
	int NumOfLogs = GetNumOfLogs();
	if (NumOfLogs >= 100) {
		LockTable(_T("Log"), LOCK_EXCLUSIVE);
		AzSortRecord(_T("Log"), _T("Id"));
		RecordData* RecDatLog = GetRecord(_T("Log"));
		UnlockTable(_T("Log"));
		int ExceededNumOfLogs = NumOfLogs - 99;
		RecordData* CurrRecDat = RecDatLog;
		for (int Loop = 0; Loop < ExceededNumOfLogs; Loop++) {
			ColumnDataInt* ColDatId = (ColumnDataInt*)CurrRecDat->GetColumn(0);
			int ValueId = ColDatId->GetValue();

			ColumnData* DelColDat[1];
			DelColDat[0] = new ColumnDataInt(_T("Id"), ValueId);
			RecordData* DelRecDat = new RecordData(_T("Log"), DelColDat, 1);
			LockTable(_T("Log"), LOCK_EXCLUSIVE);
			DeleteRecord(DelRecDat);
			UnlockTable(_T("Log"));
			delete DelRecDat;

			CurrRecDat = CurrRecDat->GetNextRecord();
		}
		delete RecDatLog;
	}

	return 0;
}

// Configure ODBC connection string
// ConnStr [in] : Connection string
void DataAccess::SetOdbcConnStr(int DbmsType, TCHAR ConnStr[256])
{
	// Record for update
	ColumnData *ColDatUpd[3];
	ColDatUpd[0] = new ColumnDataWStr(_T("ConnStr"), ConnStr);
	ColDatUpd[1] = new ColumnDataInt(_T("DbmsType"), DbmsType);
	ColDatUpd[2] = new ColumnDataInt(_T("Init"), 0);
	RecordData* RecDatUpd = new RecordData(_T("OdbcConfig"), ColDatUpd, 3);

	// Record for search
	ColumnData *ColDatSch[1];
	ColDatSch[0] = new ColumnDataInt(_T("OdbcId"), 0);
	RecordData* RecDatSch = new RecordData(_T("OdbcConfig"), ColDatSch, 1);

	// Add record
	LockTable(_T("OdbcConfig"), LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSch, RecDatUpd);
	UnlockTable(_T("OdbcConfig"));
	delete RecDatSch;
	delete RecDatUpd;
	return;
}

// Acuqire ODBC configuration
// ConnStr [out] : Acquired connection string
// Init [out] : Init flag (1:After installation, 0:After connection string is configured)
// Return : Type of DBMS (0:MariaDB, 1:PostgreSQL, 2:MySQL, -1:Connection string does not exist)
int DataAccess::GetOdbcConfing(TCHAR ConnStr[256], int* Init)
{
	LockTable(_T("OdbcConfig"), LOCK_SHARE);
	RecordData* RecDatOdbcConfig = GetRecord(_T("OdbcConfig"));
	UnlockTable(_T("OdbcConfig"));
	ColumnDataInt* ColDatOdbcId = (ColumnDataInt*)RecDatOdbcConfig->GetColumn(0);
	ColumnDataInt* ColDatDbmsType = (ColumnDataInt*)RecDatOdbcConfig->GetColumn(1);
	ColumnDataWStr* ColDatConnStr = (ColumnDataWStr*)RecDatOdbcConfig->GetColumn(2);
	ColumnDataInt* ColDatInit = (ColumnDataInt*)RecDatOdbcConfig->GetColumn(3);
	if (ColDatOdbcId == NULL || ColDatDbmsType == NULL || ColDatConnStr == NULL || ColDatInit == NULL) {
		return -1;
	}

	int OdbcId = ColDatOdbcId->GetValue();
	int DbmsType = ColDatDbmsType->GetValue();
	lstrcpy(ConnStr, ColDatConnStr->GetValue());
	*Init = ColDatInit->GetValue();
	delete RecDatOdbcConfig;
	return DbmsType;
}

// Set filter conditions
// Index [in] : each number of 1 to 5
// ColumnName [in] : Column name which filter condition
// FilterOpeType[in] : Comparison type
// Value [in] : Value which filter condition
void DataAccess::SetFilterCondition(int Index, TCHAR ColumnName[Global::COLUMNNAME_LENGTH], int FilterOpeType, TCHAR Value[Global::COLUMNVAL_LENGTH])
{
	ColumnData *ColDatFilter[4];
	ColDatFilter[0] = new ColumnDataInt(_T("Index"), Index);
	ColDatFilter[1] = new ColumnDataWStr(_T("Column"), ColumnName);
	ColDatFilter[2] = new ColumnDataInt(_T("Operation"), FilterOpeType);
	ColDatFilter[3] = new ColumnDataWStr(_T("Value"), Value);
	RecordData* RecDatFilter = new RecordData(_T("Filter"), ColDatFilter, 4);

	ColumnData *ColDatSearch[1];
	ColDatSearch[0] = new ColumnDataInt(_T("Index"), Index);
	RecordData* RecDatSearch = new RecordData(_T("Filter"), ColDatSearch, 1);

	LockTable(_T("Filter"), LOCK_EXCLUSIVE);
	UpdateRecord(RecDatSearch, RecDatFilter);
	UnlockTable(_T("Filter"));
	delete RecDatSearch;
	delete RecDatFilter;
}

// Get filter conditions
// Index [in] : each number of 1 to 5
// ColumnName [out] : Column name which filter condition
// FilterOpeType[out] : pointer to comparison type
// Value [out] : Value which filter condition
void DataAccess::GetFilterCondition(int Index, TCHAR ColumnName[Global::COLUMNNAME_LENGTH], int* FilterOpeType, TCHAR Value[Global::COLUMNVAL_LENGTH])
{
	LockTable(_T("Filter"), LOCK_SHARE);
	RecordData* RecDatFilter = GetRecord(_T("Filter"));
	UnlockTable(_T("Filter"));
	RecordData* CurRecDat = RecDatFilter;
	while (CurRecDat) {
		ColumnDataInt* ColDatIndex = (ColumnDataInt*)CurRecDat->GetColumn(0);
		ColumnDataWStr* ColDatColumnName = (ColumnDataWStr*)CurRecDat->GetColumn(1);
		ColumnDataInt* ColDatOpeType = (ColumnDataInt*)CurRecDat->GetColumn(2);
		ColumnDataWStr* ColDatValue = (ColumnDataWStr*)CurRecDat->GetColumn(3);
		if (Index == ColDatIndex->GetValue()) {
			lstrcpy(ColumnName, ColDatColumnName->GetValue());
			*FilterOpeType = ColDatOpeType->GetValue();
			lstrcpy(Value, ColDatValue->GetValue());
			break;
		}
		CurRecDat = CurRecDat->GetNextRecord();
	}
	delete RecDatFilter;
}

// Set filtering switch
// Switch [in] : TRUE:On, FALSE:Off
void DataAccess::SetFilterSwitch(BOOL Switch)
{
	// Record for update
	ColumnData *ColDatUpd[1];
	ColDatUpd[0] = new ColumnDataInt(_T("Switch"), (Switch == TRUE)? 1:0 );
	RecordData* RecDatUpd = new RecordData(_T("FilterSw"), ColDatUpd, 1);

	// Record for search
	ColumnData *ColDatSch[1];
	ColDatSch[0] = new ColumnDataInt(_T("Switch"), (Switch == TRUE)? 0:1 );
	RecordData* RecDatSch = new RecordData(_T("FilterSw"), ColDatSch, 1);

	// Add record
	LockTable(_T("FilterSw"), LOCK_EXCLUSIVE);
	int Ret = UpdateRecord(RecDatSch, RecDatUpd);
	UnlockTable(_T("FilterSw"));
	delete RecDatSch;
	delete RecDatUpd;
	return;
}

// Get filtering switch
// return : TRUE:On, FALSE:Off
BOOL DataAccess::GetFilterSwitch()
{
	LockTable(_T("FilterSw"), LOCK_SHARE);
	RecordData* RecDatFilterSw = GetRecord(_T("FilterSw"));
	UnlockTable(_T("FilterSw"));

	ColumnDataInt* ColDatSwitch = (ColumnDataInt*)RecDatFilterSw->GetColumn(0);
	if (ColDatSwitch == NULL) {
		return -1;
	}
	int Switch = ColDatSwitch->GetValue();
	delete RecDatFilterSw;
	return (Switch == 1)? TRUE : FALSE;
}

// Stops AutoSave function and save the latest data
// Return : always zero returned
int DataAccess::StopAutoSave()
{
	TCHAR Buf[MAX_PATH];
	StkGeneric::GetInstance()->GetFullPathFromFileName(DataFileName, Buf);
	AutoSave(Buf, 30, FALSE);
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
	TCHAR Buf[MAX_PATH];
	StkGeneric::GetInstance()->GetFullPathFromFileName(DataFileName, Buf);
	AutoSave(Buf, 30, TRUE);

	LockAllTable(2);
	if (StkGeneric::GetInstance()->GetFileSize(DataFileName) == 0) {

		// OdbcConfig table
		ColumnDefInt ColDefOdbcId(_T("OdbcId"));
		ColumnDefInt ColDefDbmsType(_T("DbmsType"));
		ColumnDefWStr ColDefConnStr(_T("ConnStr"), 256);
		ColumnDefInt ColDefOdbcInit(_T("Init"));
		TableDef TabDefOdbcConfig(_T("OdbcConfig"), 50);
		TabDefOdbcConfig.AddColumnDef(&ColDefOdbcId);
		TabDefOdbcConfig.AddColumnDef(&ColDefDbmsType);
		TabDefOdbcConfig.AddColumnDef(&ColDefConnStr);
		TabDefOdbcConfig.AddColumnDef(&ColDefOdbcInit);
		if (CreateTable(&TabDefOdbcConfig) != 0) {
			return -1;
		}

		// Log table
		ColumnDefInt ColDefLogId(_T("Id"));
		ColumnDefWStr ColDefLogTime(_T("Time"), Global::MAXLEN_OF_LOGTIME);
		ColumnDefWStr ColDefLogMsgEn(_T("MessageEn"), Global::MAXLEN_OF_LOGMSG);
		ColumnDefWStr ColDefLogMsgJa(_T("MessageJa"), Global::MAXLEN_OF_LOGMSG);
		TableDef TabDefLog(_T("Log"), Global::MAXNUM_OF_LOGRECORDS);
		TabDefLog.AddColumnDef(&ColDefLogId);
		TabDefLog.AddColumnDef(&ColDefLogTime);
		TabDefLog.AddColumnDef(&ColDefLogMsgEn);
		TabDefLog.AddColumnDef(&ColDefLogMsgJa);
		if (CreateTable(&TabDefLog) != 0) {
			return -1;
		}

		// FilterSw table
		ColumnDefInt ColDefSwitch(_T("Switch"));
		TableDef TabDefFilterSw(_T("FilterSw"), 5);
		TabDefFilterSw.AddColumnDef(&ColDefSwitch);
		if (CreateTable(&TabDefFilterSw) != 0) {
			return -1;
		}

		// Filter table
		ColumnDefInt ColDefIndex(_T("Index"));
		ColumnDefWStr ColDefColumn(_T("Column"), Global::COLUMNNAME_LENGTH);
		ColumnDefInt ColDefOperation(_T("Operation"));
		ColumnDefWStr ColDefValue(_T("Value"), Global::COLUMNVAL_LENGTH);
		TableDef TabDefFilter(_T("Filter"), 10);
		TabDefFilter.AddColumnDef(&ColDefIndex);
		TabDefFilter.AddColumnDef(&ColDefColumn);
		TabDefFilter.AddColumnDef(&ColDefOperation);
		TabDefFilter.AddColumnDef(&ColDefValue);
		if (CreateTable(&TabDefFilter) != 0) {
			return -1;
		}

		int Ret;

		// Add record for OdbcConfig
		ColumnData *ColDatOdbcConfig[4];
		ColDatOdbcConfig[0] = new ColumnDataInt(_T("OdbcId"), 0);
		ColDatOdbcConfig[1] = new ColumnDataInt(_T("DbmsType"), 0);
		ColDatOdbcConfig[2] = new ColumnDataWStr(_T("ConnStr"), _T("Driver={MySQL ODBC 5.2 Unicode Driver};Server=localhost;Port=3306;Option=131072;Stmt=;Database=DATABASE_NAME;Uid=UID;Pwd=PWD;"));
		ColDatOdbcConfig[3] = new ColumnDataInt(_T("Init"), 1);
		RecordData* RecDatOdbcConfig = new RecordData(_T("OdbcConfig"), ColDatOdbcConfig, 4);
		LockTable(_T("OdbcConfig"), LOCK_EXCLUSIVE);
		Ret = InsertRecord(RecDatOdbcConfig);
		UnlockTable(_T("OdbcConfig"));
		delete RecDatOdbcConfig;

		// Add record for FilterSw
		ColumnData *ColDatFilterSw[1];
		ColDatFilterSw[0] = new ColumnDataInt(_T("Switch"), 0);
		RecordData* RecDatFilterSw = new RecordData(_T("FilterSw"), ColDatFilterSw, 1);
		LockTable(_T("FilterSw"), LOCK_EXCLUSIVE);
		Ret = InsertRecord(RecDatFilterSw);
		UnlockTable(_T("FilterSw"));
		delete RecDatFilterSw;

		// Add record for Filter
		for (int Loop = 1; Loop <= 5; Loop++) {
			ColumnData *ColDatFilter[4];
			ColDatFilter[0] = new ColumnDataInt(_T("Index"), Loop);
			ColDatFilter[1] = new ColumnDataWStr(_T("Column"), _T("*"));
			ColDatFilter[2] = new ColumnDataInt(_T("Operation"), 0);
			ColDatFilter[3] = new ColumnDataWStr(_T("Value"), _T(""));
			RecordData* RecDatFilter = new RecordData(_T("Filter"), ColDatFilter, 4);
			LockTable(_T("Filter"), LOCK_EXCLUSIVE);
			Ret = InsertRecord(RecDatFilter);
			UnlockTable(_T("Filter"));
			delete RecDatFilter;
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
