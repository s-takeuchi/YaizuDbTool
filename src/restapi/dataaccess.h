#pragma once
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "Global.h"

class DataAccess
{
private:
	// This instance
	static DataAccess* ThisInstance;
	wchar_t DataFileName[FILENAME_MAX];
	DataAccess();
	~DataAccess();

	static const int LatestDbVersion = 3;

public:
	// Get this instance
	static DataAccess* GetInstance();
	int StopAutoSave();
	int CreateCmdFreakTables();
	void SetOdbcConnStr(int, wchar_t[256]);
	int GetOdbcConfing(wchar_t[256], int*);
	void SetFilterCondition(int, wchar_t[Global::COLUMNNAME_LENGTH], int, wchar_t[Global::COLUMNVAL_LENGTH]);
	void GetFilterCondition(int, wchar_t[Global::COLUMNNAME_LENGTH], int*, wchar_t[Global::COLUMNVAL_LENGTH]);
	void SetFilterSwitch(bool);
	bool GetFilterSwitch();
	bool GetTargetUserByName(wchar_t[Global::MAXLEN_OF_USERNAME], int*, wchar_t[Global::MAXLEN_OF_PASSWORD], int*);
	bool GetTargetUserById(int, wchar_t[Global::MAXLEN_OF_USERNAME], wchar_t[Global::MAXLEN_OF_PASSWORD], int*);
	int GetTargetUsers(int[Global::MAXNUM_OF_USERRECORDS], wchar_t[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_USERNAME], wchar_t[Global::MAXNUM_OF_USERRECORDS][Global::MAXLEN_OF_PASSWORD], int[Global::MAXNUM_OF_USERRECORDS]);
	int GetNumberOfUsers();
	bool AddUser(wchar_t[Global::MAXLEN_OF_USERNAME], int, wchar_t[Global::MAXLEN_OF_PASSWORD]);
	bool UpdateUser(int, wchar_t[Global::MAXLEN_OF_USERNAME], int, wchar_t[Global::MAXLEN_OF_PASSWORD]);
	bool DeleteUser(int);

	int GetDbVersion();
	int DbUpdate_NonVer_V3();
};
