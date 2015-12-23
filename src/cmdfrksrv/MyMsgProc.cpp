#include "..\..\..\YaizuComLib\src\\commonfunc\msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================
	//3100 (---)
	MessageProc::AddJpn(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2014 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));
	MessageProc::AddEng(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2014 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));

	//3126 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DBMSCHANGE, _T("ODBC接続の設定を変更しました。"));
	MessageProc::AddEng(CMDFRK_LOG_DBMSCHANGE, _T("The ODBC connection configuration has been changed."));

	//3129 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTERINGCOND_UPDATE, _T("フィルタリング条件を更新しました。"));
	MessageProc::AddEng(CMDFRK_FILTERINGCOND_UPDATE, _T("The filter conditions have been updated."));

	//3101 (NC6)
	MessageProc::AddJpn(CMDFRK_CANNOT_START, _T("\r\nこのプログラムはWindowsサービスプログラムのため起動することができません。\r\nこのプログラムはCmdFreakのインストール時にWindowsサービスとしてシステムに登録されます。\r\n\r\n"));
	MessageProc::AddEng(CMDFRK_CANNOT_START, _T("\r\nThis program cannot be started because it is configured as a Windows service program.\r\nThe program is registered as a Windows service in the system when CmdFreak is installed.\r\n\r\n"));

	//3146 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTART, _T("CmdFreakサービスを開始しました。"));
	MessageProc::AddEng(CMDFRK_SVCSTART, _T("CmdFreak service has started."));

	//3147 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTOP, _T("CmdFreakサービスを停止しました。"));
	MessageProc::AddEng(CMDFRK_SVCSTOP, _T("CmdFreak service has stopped."));

	//3226 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGTIME, _T("ログ出力時刻"));
	MessageProc::AddEng(CMDFRK_LOGGINGTIME, _T("Logging time"));

	//3227 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGMSG, _T("メッセージ"));
	MessageProc::AddEng(CMDFRK_LOGGINGMSG, _T("Message"));

}

TCHAR* MyMsgProc::GetMsg(int Id)
{
	return MessageProc::GetMsg(Id);
}

BYTE* MyMsgProc::GetMsgSjis(int Id)
{
	return MessageProc::GetMsgSjis(Id);
}

void MyMsgProc::StkErr(int Id, HWND WndHndl)
{
	MessageProc::StkErr(Id, WndHndl);
}

void MyMsgProc::StkErr(int Id, TCHAR* Str, HWND WndHndl)
{
	MessageProc::StkErr(Id, Str, WndHndl);
}

void MyMsgProc::StkInf(int Id, HWND WndHndl)
{
	MessageProc::StkInf(Id, WndHndl);
}

void MyMsgProc::StkInf(int Id, TCHAR* Str, HWND WndHndl)
{
	MessageProc::StkInf(Id, Str, WndHndl);
}

int MyMsgProc::StkYesNo(int Id, HWND WndHndl)
{
	return MessageProc::StkYesNo(Id, WndHndl);
}

int MyMsgProc::StkYesNo(int Id, TCHAR* Str, HWND WndHndl)
{
	return MessageProc::StkYesNo(Id, Str, WndHndl);
}
