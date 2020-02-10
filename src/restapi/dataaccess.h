#pragma once
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "..\Global.h"

class DataAccess
{
private:
	// This instance
	static DataAccess* ThisInstance;
	wchar_t DataFileName[FILENAME_MAX];
	DataAccess();
	~DataAccess();

public:
	// Get this instance
	static DataAccess* GetInstance();
	int StopAutoSave();
	int CreateCmdFreakTables();
	int AddLogMsg(wchar_t[Global::MAXLEN_OF_LOGMSG], wchar_t[Global::MAXLEN_OF_LOGMSG]);
	int GetMaxLogId();
	int GetNumOfLogs();
	int GetLogs(wchar_t[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGTIME], wchar_t[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG], wchar_t[Global::MAXNUM_OF_LOGRECORDS][Global::MAXLEN_OF_LOGMSG]);
	int DeleteOldLogs();
	void SetOdbcConnStr(int, wchar_t[256]);
	int GetOdbcConfing(wchar_t[256], int*);
	void SetFilterCondition(int, wchar_t[Global::COLUMNNAME_LENGTH], int, wchar_t[Global::COLUMNVAL_LENGTH]);
	void GetFilterCondition(int, wchar_t[Global::COLUMNNAME_LENGTH], int*, wchar_t[Global::COLUMNVAL_LENGTH]);
	void SetFilterSwitch(bool);
	bool GetFilterSwitch();
	bool GetTargetUserByName(wchar_t[Global::MAXLEN_OF_USERNAME], int*, wchar_t[Global::MAXLEN_OF_PASSWORD], int*);
	int GetTargetUsers(int[Global::MAXNUM_OF_USERRECORDS], wchar_t[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_USERNAME], wchar_t[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_PASSWORD], int[Global::MAXNUM_OF_USERRECORDS]);
	bool UpdateUser(int, wchar_t[Global::MAXLEN_OF_USERNAME], int);
};
