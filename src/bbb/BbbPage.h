#pragma once

#include <windows.h>
#include "..\Global.h"

class BbbPage
{
private:
	// This instance
	static BbbPage* ThisInstance;
	BbbPage();
	~BbbPage();

	void WriteText(TCHAR*);
	void WriteTxtP(TCHAR*, TCHAR*);
	void WriteTxtP(TCHAR*, int);
	void WriteTxtQ(TCHAR*, TCHAR*, TCHAR*);

	void OutputTitle();
	void OutputHeadStart();
	void OutputHeadEnd();
	void OutputHeadBase();
	void OutputHeadMain(int, TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH]);
	void OutputBodyMainStart();
	void OutputBodyMainEnd();
	void OutputMain(int, TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH]);

public:
	static const int MAINPAGE_INIT = 0;
	static const int MAINPAGE_UNEXPECTED_ERROR = 1100;

	static const int MAX_OPERATION_LENGTH = 64;

	static const int RESULTCODE_SUCCESS = 0;
	static const int RESULTCODE_UNEXPECTED_ERROR = 1100;
	static const int RESULTCODE_SERVICEDOWN_ERROR = 1110;
	static const int RESULTCODE_DBMSCONNSTR_ERROR = 1120;
	static const int RESULTCODE_EMPTYCONNSTR_ERROR = 1130;
	static const int RESULTCODE_TOOMUCHRECORD_ERROR = 1140;
	static const int RESULTCODE_TOOLONGCONNSTR_ERROR = 1150;
	static const int RESULTCODE_TOOLONGFILTER_ERROR = 1160;

public:
	// Get this instance
	static BbbPage* GetInstance();

	void MainPage(int, TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::MAX_PARAM_LENGTH]);
	void OutputCommandResult(TCHAR[BbbPage::MAX_OPERATION_LENGTH], int, TCHAR[Global::MAX_PARAM_LENGTH]);

};
