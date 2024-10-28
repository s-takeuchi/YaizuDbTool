#pragma once

class MyMsgProc
{
public:
	// Errors
	static const int CMDFRK_REQ_NOT_SUFFICIENT        = 3121;
	static const int CMDFRK_TABLE_NOT_EXIST           = 3122;
	static const int CMDFRK_PARAM_LENGTH_TOO_LONG     = 3123;
	static const int CMDFRK_AUTH_ERROR                = 3124;
	static const int CMDFRK_ACCESS_RIGHT_ERROR        = 3125;

	// Logging
	static const int CMDFRK_LOG_DBMSCHANGE            = 3336;
	static const int CMDFRK_LOG_FILTERCHANGE          = 3339;
	static const int CMDFRK_SVCSTART                  = 3346;
	static const int CMDFRK_SVCSTOP                   = 3347;

	static void AddMsg();
	static wchar_t* GetMsg(int);
	static char* GetMsgSjis(int);
	static wchar_t* GetMsgJpn(int);
	static wchar_t* GetMsgEng(int);
};
