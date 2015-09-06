#pragma once

class MyMsgProc
{
public:
	static const int CMDFRK_CMDFRKSRV = 3100;
	static const int CMDFRK_CANNOT_START = 3101;
	static const int CMDFRK_LOG_DBMSCHANGE = 3126;
	static const int CMDFRK_FILTERINGCOND_UPDATE = 3129;
	static const int CMDFRK_SVCSTART = 3146;
	static const int CMDFRK_SVCSTOP = 3147;
	static const int CMDFRK_LOGGINGTIME = 3226;
	static const int CMDFRK_LOGGINGMSG = 3227;

	static void AddMsg();
	static TCHAR* GetMsg(int);
	static BYTE* GetMsgSjis(int);
	static void StkErr(int, HWND);
	static void StkErr(int, TCHAR*, HWND);
	static void StkInf(int, HWND);
	static void StkInf(int, TCHAR*, HWND);
	static int StkYesNo(int, HWND);
	static int StkYesNo(int, TCHAR*, HWND);
};
