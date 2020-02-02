#pragma once

class MyMsgProc
{
public:
	static const int CMDFRK_CMDFRKSRV = 3100;
	static const int CMDFRK_CONF_NOT_FOUND = 3111;
	static const int CMDFRK_DAT_NOT_FOUND = 3112;
	static const int CMDFRK_DAT_SERVICEHOST_NOT_FOUND = 3113;
	static const int CMDFRK_DAT_SERVICEPORT_NOT_FOUND = 3114;
	static const int CMDFRK_EXEC_NOT_FOUND = 3115;
	static const int CMDFRK_NO_CLIENTLOCALE = 3120;
	static const int CMDFRK_REQ_NOT_SUFFICIENT = 3121;
	static const int CMDFRK_TABLE_NOT_EXIST = 3122;
	static const int CMDFRK_PARAM_LENGTH_TOO_LONG = 3123;
	static const int CMDFRK_AUTH_ERROR = 3124;
	static const int CMDFRK_ACCESS_RIGHT_ERROR = 3125;
	static const int CMDFRK_LOG_DBMSCHANGE = 3126;
	static const int CMDFRK_LOG_FILTERCHANGE = 3129;
	static const int CMDFRK_SVCSTART = 3146;
	static const int CMDFRK_SVCSTOP = 3147;
	static const int CMDFRK_LOGGINGTIME = 3226;
	static const int CMDFRK_LOGGINGMSG = 3227;

	static void AddMsg();
	static wchar_t* GetMsg(int);
	static char* GetMsgSjis(int);
	static wchar_t* GetMsgJpn(int);
	static wchar_t* GetMsgEng(int);
};
