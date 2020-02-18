#include "..\..\..\YaizuComLib\src\\commonfunc\msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================
	//(---)
	MessageProc::AddJpn(CMDFRK_CMDFRKSRV, L"CmdFreak service program\r\nCopyright (C) 2019 Shinya Takeuchi\r\nAll Rights Reserved.\r\n");
	MessageProc::AddEng(CMDFRK_CMDFRKSRV, L"CmdFreak service program\r\nCopyright (C) 2019 Shinya Takeuchi\r\nAll Rights Reserved.\r\n");

	//(★)
	MessageProc::AddJpn(CMDFRK_CONF_NOT_FOUND, L"sample.conf が見つかりません。\r\n");
	MessageProc::AddEng(CMDFRK_CONF_NOT_FOUND, L"sample.conf does not found.\r\n");

	//(★)
	MessageProc::AddJpn(CMDFRK_DAT_NOT_FOUND, L"sample.dat が見つかりません。\r\n");
	MessageProc::AddEng(CMDFRK_DAT_NOT_FOUND, L"sample.dat does not found.\r\n");

	//(★)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, L"servicehost がsample.confの中に見つかりません。\r\n");
	MessageProc::AddEng(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, L"servicehost does not found in sample.conf.\r\n");

	//(★)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, L"serviceport がsample.confの中に見つかりません。\r\n");
	MessageProc::AddEng(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, L"serviceport does not found in sample.conf.\r\n");

	//(★)
	MessageProc::AddJpn(CMDFRK_EXEC_NOT_FOUND, L"実行ファイルが見つかりません。\r\n");
	MessageProc::AddEng(CMDFRK_EXEC_NOT_FOUND, L"The executable file does not found.\r\n");

	//
	// Errors
	//

	//(★)
	MessageProc::AddJpn(CMDFRK_NO_CLIENTLOCALE, L"クライアントのロケールを検知できません。");
	MessageProc::AddEng(CMDFRK_NO_CLIENTLOCALE, L"Client locale cannot be detected.");

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
	MessageProc::AddJpn(CMDFRK_CANNOT_MODIFY_YOUR_INFO, L"自分自身のユーザー情報を変更することはできません。");
	MessageProc::AddEng(CMDFRK_CANNOT_MODIFY_YOUR_INFO, L"You cannot modify user information yourself.");

	//(★)
	MessageProc::AddJpn(CMDFRK_SAME_USER_NAME_EXIST, L"すでに同名のユーザーが存在するため，ユーザー情報を変更できません。");
	MessageProc::AddEng(CMDFRK_SAME_USER_NAME_EXIST, L"You cannot modify user information because the user who has the same name exists.");

	//(★)
	MessageProc::AddJpn(CMDFRK_USER_DOES_NOT_EXIST, L"指定したユーザーは存在しません。");
	MessageProc::AddEng(CMDFRK_USER_DOES_NOT_EXIST, L"The specified user does not exist.");

	//(★)
	MessageProc::AddJpn(CMDFRK_INVALID_USER_NAME, L"指定されたユーザー名に使用禁止文字が含まれています。");
	MessageProc::AddEng(CMDFRK_INVALID_USER_NAME, L"The specified user name contains prohibited character(s).");

	//(★)
	MessageProc::AddJpn(CMDFRK_INVALID_LEN_OF_USER_NAME, L"ユーザー名は4文字以上である必要があります。");
	MessageProc::AddEng(CMDFRK_INVALID_LEN_OF_USER_NAME, L"The user name needs to contain at least 4 characters.");

	//(★)
	MessageProc::AddJpn(CMDFRK_INVALID_PASSWORD, L"指定されたパスワードに使用禁止文字が含まれています。");
	MessageProc::AddEng(CMDFRK_INVALID_PASSWORD, L"The specified password contains prohibited character(s).");

	//(★)
	MessageProc::AddJpn(CMDFRK_INVALID_LEN_OF_PASSWORD, L"パスワードは4文字以上である必要があります。");
	MessageProc::AddEng(CMDFRK_INVALID_LEN_OF_PASSWORD, L"The password needs to contain at least 4 characters.");

	//
	// Logging
	//

	//(★)
	MessageProc::AddJpn(CMDFRK_USER_ADD, L"新規にユーザーを追加しました。 [%ls]");
	MessageProc::AddEng(CMDFRK_USER_ADD, L"New user has been added. [%ls]");

	//(★)
	MessageProc::AddJpn(CMDFRK_USER_DELETE, L"ユーザーを削除しました。 [%ls]");
	MessageProc::AddEng(CMDFRK_USER_DELETE, L"A user has been deleted. [%ls]");

	//(★)
	MessageProc::AddJpn(CMDFRK_USER_EDIT, L"ユーザー情報を変更しました。 [%ls]");
	MessageProc::AddEng(CMDFRK_USER_EDIT, L"A user information has been modified. [%ls]");

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
