#pragma once

class MyMsgProc
{
public:
	// Command line message
	static const int CMDFRK_CMDFRKSRV                 = 3100;
	static const int CMDFRK_CONF_NOT_FOUND            = 3111;
	static const int CMDFRK_DAT_NOT_FOUND             = 3112;
	static const int CMDFRK_DAT_SERVICEHOST_NOT_FOUND = 3113;
	static const int CMDFRK_DAT_SERVICEPORT_NOT_FOUND = 3114;
	static const int CMDFRK_EXEC_NOT_FOUND            = 3115;

	// Errors
	static const int CMDFRK_NO_CLIENTLOCALE           = 3120;
	static const int CMDFRK_REQ_NOT_SUFFICIENT        = 3121;
	static const int CMDFRK_TABLE_NOT_EXIST           = 3122;
	static const int CMDFRK_PARAM_LENGTH_TOO_LONG     = 3123;
	static const int CMDFRK_AUTH_ERROR                = 3124;
	static const int CMDFRK_ACCESS_RIGHT_ERROR        = 3125;
	static const int CMDFRK_CANNOT_MODIFY_YOUR_INFO   = 3126;
	static const int CMDFRK_SAME_USER_NAME_EXIST      = 3127;
	static const int CMDFRK_USER_DOES_NOT_EXIST       = 3128;
	static const int CMDFRK_INVALID_USER_NAME         = 3129;
	static const int CMDFRK_INVALID_LEN_OF_USER_NAME  = 3130;
	static const int CMDFRK_INVALID_PASSWORD          = 3131;
	static const int CMDFRK_INVALID_LEN_OF_PASSWORD   = 3132;
	static const int CMDFRK_PASSWORD_REQUIRED_FOR_ADD = 3133;
	static const int CMDFRK_EXCEEDED_MAX_NUM_OF_USER  = 3134;

	// Logging
	static const int CMDFRK_USER_ADD                  = 3331;
	static const int CMDFRK_USER_DELETE               = 3332;
	static const int CMDFRK_USER_EDIT                 = 3333;
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
