#include "..\..\..\YaizuComLib\src\\commonfunc\msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================
	//3100 (---)
	MessageProc::AddJpn(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2017 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));
	MessageProc::AddEng(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2017 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));

	//3111 (★)
	MessageProc::AddJpn(CMDFRK_CONF_NOT_FOUND, _T("stkwebapp.conf が見つかりません。\r\n"));
	MessageProc::AddEng(CMDFRK_CONF_NOT_FOUND, _T("stkwebapp.conf does not found.\r\n"));

	//3112 (★)
	MessageProc::AddJpn(CMDFRK_DAT_NOT_FOUND, _T("stkwebapp.dat が見つかりません。\r\n"));
	MessageProc::AddEng(CMDFRK_DAT_NOT_FOUND, _T("stkwebapp.dat does not found.\r\n"));

	//3113 (★)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, _T("servicehost がstkwebapp.confの中に見つかりません。\r\n"));
	MessageProc::AddEng(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, _T("servicehost does not found in stkwebapp.conf.\r\n"));

	//3114 (★)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, _T("serviceport がstkwebapp.confの中に見つかりません。\r\n"));
	MessageProc::AddEng(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, _T("serviceport does not found in stkwebapp.conf.\r\n"));

	//3120 (★)
	MessageProc::AddJpn(CMDFRK_NO_CLIENTLOCALE, _T("クライアントのロケールを検知できません。"));
	MessageProc::AddEng(CMDFRK_NO_CLIENTLOCALE, _T("Client locale cannot be detected."));

	//3121 (★)
	MessageProc::AddJpn(CMDFRK_REQ_NOT_SUFFICIENT, _T("必要なパラメータがリクエストに含まれていません。"));
	MessageProc::AddEng(CMDFRK_REQ_NOT_SUFFICIENT, _T("The necessary parameter(s) is/are not contained in the request."));

	//3122 (★)
	MessageProc::AddJpn(CMDFRK_TABLE_NOT_EXIST, _T("指定したテーブルが存在しません。"));
	MessageProc::AddEng(CMDFRK_TABLE_NOT_EXIST, _T("The specified table does not exist."));

	//3123 (★)
	MessageProc::AddJpn(CMDFRK_PARAM_LENGTH_TOO_LONG, _T("指定されたパラメータは，指定可能な長さの最大を超えています。"));
	MessageProc::AddEng(CMDFRK_PARAM_LENGTH_TOO_LONG, _T("The specified parameter exceeds the maximum length."));

	//3126 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DBMSCHANGE, _T("ODBC接続の設定を変更しました。"));
	MessageProc::AddEng(CMDFRK_LOG_DBMSCHANGE, _T("The ODBC connection configuration has been changed."));

	//3129 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTERINGCOND_UPDATE, _T("フィルタリング条件を更新しました。"));
	MessageProc::AddEng(CMDFRK_FILTERINGCOND_UPDATE, _T("The filter conditions have been updated."));

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

TCHAR* MyMsgProc::GetMsgJpn(int Id)
{
	return MessageProc::GetMsgJpn(Id);
}

TCHAR* MyMsgProc::GetMsgEng(int Id)
{
	return MessageProc::GetMsgEng(Id);
}
