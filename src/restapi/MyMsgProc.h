#pragma once

class MyMsgProc
{
public:
	static const int CMDFRK_CMDFRKSRV = 3100;
	static const int CMDFRK_CONF_NOT_FOUND = 3111;
	static const int CMDFRK_DAT_NOT_FOUND = 3112;
	static const int CMDFRK_DAT_SERVICEHOST_NOT_FOUND = 3113;
	static const int CMDFRK_DAT_SERVICEPORT_NOT_FOUND = 3114;
	static const int CMDFRK_LOG_DBMSCHANGE = 3126;
	static const int CMDFRK_FILTERINGCOND_UPDATE = 3129;
	static const int CMDFRK_SVCSTART = 3146;
	static const int CMDFRK_SVCSTOP = 3147;
	static const int CMDFRK_LOGGINGTIME = 3226;
	static const int CMDFRK_LOGGINGMSG = 3227;

	static void AddMsg();
	static BYTE* GetMsgSjis(int);
	static TCHAR* GetMsgJpn(int);
	static TCHAR* GetMsgEng(int);
};
