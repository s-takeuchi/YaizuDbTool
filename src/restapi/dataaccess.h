#pragma once
#include <windows.h>
#include <tchar.h>
#include "..\Global.h"

class DataAccess
{
private:
	// This instance
	static DataAccess* ThisInstance;
	TCHAR DataFileName[MAX_PATH];
	DataAccess();
	~DataAccess();

public:
	// Get this instance
	static DataAccess* GetInstance();
	int StopAutoSave();
	int CreateCmdFreakTables();
	int AddLogMsg(TCHAR[Global::MAXLEN_OF_LOGMSG], TCHAR[Global::MAXLEN_OF_LOGMSG]);
	int GetMaxLogId();
	int GetNumOfLogs();
	int GetLogs(TCHAR[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGTIME], TCHAR[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG], TCHAR[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG]);
	int GetLogAsHtml(TCHAR[Global::MAX_PARAM_LENGTH / 2]);
	int DeleteOldLogs();
	void SetOdbcConnStr(int, TCHAR[256]);
	int GetOdbcConfing(TCHAR[256], int*);
	void SetFilterCondition(int, TCHAR[Global::COLUMNNAME_LENGTH], int, TCHAR[Global::COLUMNVAL_LENGTH]);
	void GetFilterCondition(int, TCHAR[Global::COLUMNNAME_LENGTH], int*, TCHAR[Global::COLUMNVAL_LENGTH]);
	void SetFilterSwitch(BOOL);
	BOOL GetFilterSwitch();
};
