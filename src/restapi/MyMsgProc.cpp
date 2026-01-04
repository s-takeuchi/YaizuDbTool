#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================

	//
	// Information / Error messages
	//

	//(★)
	MessageProc::AddJpn(CMDFRK_REQ_NOT_SUFFICIENT, L"必要なパラメータがリクエストに含まれていません。");
	MessageProc::AddEng(CMDFRK_REQ_NOT_SUFFICIENT, L"The necessary parameter(s) is/are not contained in the request.");

	//(★)
	MessageProc::AddJpn(CMDFRK_TABLE_NOT_EXIST, L"指定したテーブルが存在しません。");
	MessageProc::AddEng(CMDFRK_TABLE_NOT_EXIST, L"The specified table does not exist.");

	//(★)
	MessageProc::AddJpn(CMDFRK_PARAM_LENGTH_TOO_LONG, L"指定されたパラメータは，指定可能な長さの最大を超えています。");
	MessageProc::AddEng(CMDFRK_PARAM_LENGTH_TOO_LONG, L"The specified parameter exceeds the maximum length.");

	//(★)
	MessageProc::AddJpn(CMDFRK_AUTH_ERROR, L"認証エラーが発生しました。");
	MessageProc::AddEng(CMDFRK_AUTH_ERROR, L"Authentication error occurred.");

	//(★)
	MessageProc::AddJpn(CMDFRK_ACCESS_RIGHT_ERROR, L"アクセス権限エラーが発生しました。");
	MessageProc::AddEng(CMDFRK_ACCESS_RIGHT_ERROR, L"Access right error occurred.");

	//(★)
	MessageProc::AddJpn(CMDFRK_ODBCINFO_WIN, L"\"Driver=\"には、64-bit ODBC データ ソースで表示されるドライバ名を指定してください。");
	MessageProc::AddEng(CMDFRK_ODBCINFO_WIN, L"Specify the driver name which displayed at 64bit ODBC data source to \"Driver\".");

	//(★)
	MessageProc::AddJpn(CMDFRK_ODBCINFO_LIN, L"\"Driver=\"には、odbcinst -q -d で確認できるドライバ名、またはドライバの共有ライブラリパスを指定してください。");
	MessageProc::AddEng(CMDFRK_ODBCINFO_LIN, L"Spcify the driver name which can be confirmed at \"odbcinst -q -d\" or the path to the shared library to \"Driver=\".");

	//
	// Logging messages
	//

	//(NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DBMSCHANGE, L"ODBC接続の設定を変更しました。");
	MessageProc::AddEng(CMDFRK_LOG_DBMSCHANGE, L"The ODBC connection configuration has been changed.");

	//(NC6)
	MessageProc::AddJpn(CMDFRK_LOG_FILTERCHANGE, L"フィルタリング条件を更新しました。");
	MessageProc::AddEng(CMDFRK_LOG_FILTERCHANGE, L"The filter criteria have been updated.");

	//(NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTART, L"CmdFreakサービスを開始しました。");
	MessageProc::AddEng(CMDFRK_SVCSTART, L"CmdFreak service has started.");

	//(NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTOP, L"CmdFreakサービスを停止しました。");
	MessageProc::AddEng(CMDFRK_SVCSTOP, L"CmdFreak service has stopped.");

}

wchar_t* MyMsgProc::GetMsg(int Id)
{
	return MessageProc::GetMsg(Id);
}

char* MyMsgProc::GetMsgSjis(int Id)
{
	return MessageProc::GetMsgSjis(Id);
}

wchar_t* MyMsgProc::GetMsgJpn(int Id)
{
	return MessageProc::GetMsgJpn(Id);
}

wchar_t* MyMsgProc::GetMsgEng(int Id)
{
	return MessageProc::GetMsgEng(Id);
}
