#include "..\..\..\YaizuComLib\src\\msgproc\msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================
	//2901 (NC2)
	MessageProc::AddJpn(UNKNOWNERROR, _T("不明なエラーが発生しました。"));
	MessageProc::AddEng(UNKNOWNERROR, _T("An unknown error has occurred."));

	//2902 (NC2)
	MessageProc::AddJpn(FILEACCESSERROR, _T("ファイルアクセスエラーが発生しました。"));
	MessageProc::AddEng(FILEACCESSERROR, _T("A file access error has occurred."));

	//2903 (NC1)
	MessageProc::AddJpn(NEEDADMINRIGHTS, _T("このプログラムを実行するためにはLocal Administratorの権限が必要です。\r\nプログラムは終了します。\r\n\r\n"));
	MessageProc::AddEng(NEEDADMINRIGHTS, _T("You need local administrator's rights to run this program.\r\nThe program will be terminated.\r\n\r\n"));

	// 2904 (NC3)
	MessageProc::AddJpn(DATAUPDATED, _T("プログラムが使用するデータは更新されています。\r\nデータが失われますが操作を続けて良いですか。"));
	MessageProc::AddEng(DATAUPDATED, _T("Program data updated.\r\nData will be lost. Continue operation?"));

	// 2905 (NC4)
	MessageProc::AddJpn(INVALIDVALUE, _T("指定した値が不正です。値の指定可能な範囲を確認してください。"));
	MessageProc::AddEng(INVALIDVALUE, _T("Invalid value specified. Check the range of values allowed."));

	//2906 (---)
	MessageProc::AddJpn(AN_ERROR_OCCURRED, _T("エラーが発生しました。"));
	MessageProc::AddEng(AN_ERROR_OCCURRED, _T("An error occurred."));

	//2931 (---)
	MessageProc::AddJpn(COMMON_ADD, _T("追加"));
	MessageProc::AddEng(COMMON_ADD, _T("Add"));

	//2932 (---)
	MessageProc::AddJpn(COMMON_DELETE, _T("削除"));
	MessageProc::AddEng(COMMON_DELETE, _T("Delete"));

	//2933 (---)
	MessageProc::AddJpn(COMMON_EDIT, _T("編集"));
	MessageProc::AddEng(COMMON_EDIT, _T("Edit"));

	//2934 (---)
	MessageProc::AddJpn(COMMON_REFRESH, _T("更新"));
	MessageProc::AddEng(COMMON_REFRESH, _T("Refresh"));

	//2935 (---)
	MessageProc::AddJpn(COMMON_TRUE, _T("True"));
	MessageProc::AddEng(COMMON_TRUE, _T("True"));

	//2936 (---)
	MessageProc::AddJpn(COMMON_FALSE, _T("False"));
	MessageProc::AddEng(COMMON_FALSE, _T("False"));

	//2937 (---)
	MessageProc::AddJpn(COMMON_UNKNOWN, _T("不明"));
	MessageProc::AddEng(COMMON_UNKNOWN, _T("Unknown"));

	//2941 (---)
	MessageProc::AddJpn(COMMON_OK, _T("OK"));
	MessageProc::AddEng(COMMON_OK, _T("OK"));

	//2942 (---)
	MessageProc::AddJpn(COMMON_CANCEL, _T("ｷｬﾝｾﾙ"));
	MessageProc::AddEng(COMMON_CANCEL, _T("Cancel"));

	// =====================================================================================================
	//3100 (---)
	MessageProc::AddJpn(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2014 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));
	MessageProc::AddEng(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2014 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));

	//3101 (NC6)
	MessageProc::AddJpn(CMDFRK_CANNOT_START, _T("\r\nこのプログラムはWindowsサービスプログラムのため起動することができません。\r\nこのプログラムはCmdFreakのインストール時にWindowsサービスとしてシステムに登録されます。\r\n\r\n"));
	MessageProc::AddEng(CMDFRK_CANNOT_START, _T("\r\nThis program cannot be started because it is configured as a Windows service program.\r\nThe program is registered as a Windows service in the system when CmdFreak is installed.\r\n\r\n"));

	//3121 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_UNEXPECTED, _T("予期しないエラーが発生しました。このエラーはWebブラウザからのリクエストが解析できなかったときに発生する場合があります。<br>"));
	MessageProc::AddEng(CMDFRK_ERROR_UNEXPECTED, _T("An unexpected error occurred. This may be caused by inability to analyze request data from the web browser.<br>"));

	//3122 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_SERVICEDOWN, _T("CmdFreakサービスとの通信が失敗しました。次の要因が考えられます。<br>(1) CmdFreakサービスが開始されていない。<br>(2) CmdFreakサービスがファイアウォールに例外登録されていない。<br>(3) 接続先ホスト名およびポート番号の定義ファイル [bbb.conf, cmdfrksrv.conf] が不正。<br>(4) CmdFreakの内部処理で異常が発生している。<br>"));
	MessageProc::AddEng(CMDFRK_ERROR_SERVICEDOWN, _T("Connection with CmdFreak service failed. This may be caused by one of the following issues:<br>(1) CmdFreak service cannot be started.<br>(2) CmdFreak service is not registered as a firewall exception.<br>(3) The definition file [bbb.conf and/or cmdfrksrv.conf] for the host name and port number in the network connectivity settings is invalid.<br>(4) A CmdFreak internal process is invalid.<br>"));

	//3123 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_JAVASCRIPTERR, _T("JavaScriptが無効です。<br>ブラウザの設定を変更してJavaScriptを有効後，ページをリフレッシュしてください。<br>\n"));
	MessageProc::AddEng(CMDFRK_ERROR_JAVASCRIPTERR, _T("JavaScript is disabled in your browser.<br>Change the browser setting to enable it and refresh the page.<br>\n"));

	//3124 (NC6)
	MessageProc::AddJpn(CMDFRK_ODBCINFO, _T("ODBC接続の設定を行います。<br>接続対象のDBMSを選択し，ODBCの接続文字列を指定してください。<br>本ソフトウェアは32ビットアプリケーションのため，ODBC接続文字列には，32ビット版ODBCドライバを使用するように指定を行ってください。オペレーティングシステムには，あらかじめ32ビット版ODBCドライバがインストールされている必要があります。<br>"));
	MessageProc::AddEng(CMDFRK_ODBCINFO, _T("Configure ODBC connection.<br>Select the connection target DBMS and specify the ODBC connection string.<br>As this is 32-bit application software, this string should be specified for the use of a 32-bit ODBC driver. As a prerequisite, a 32-bit ODBC driver needs to be installed on the operating system.<br>"));

	//3125 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_INVALIDDBCONN, _T("DBMSとの接続に失敗しました。<br>次の要因が考えられます。<br>- DBMSが適切に起動していない。<br>- ODBCドライバがインストールされていない。<br>- ODBCの接続文字列が不正。<br>- DBMSとの接続経路に何らかの問題がある。<br>"));
	MessageProc::AddEng(CMDFRK_ERROR_INVALIDDBCONN, _T("DBMS connection failed.<br>This may be caused by one of the following issues:<br>- The DBMS is not working properly.<br>- No ODBC driver is installed.<br>- The ODBC connection string is invalid.<br>- There are issues with the DBMS on the network.<br>"));

	//3126 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DBMSCHANGE, _T("ODBC接続の設定を変更しました。"));
	MessageProc::AddEng(CMDFRK_LOG_DBMSCHANGE, _T("The ODBC connection configuration has been changed."));

	//3127 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTER_DESC, _T("ここではフィルタリングの設定を行うことができます。<br>指定した条件でレコード情報がフィルタリングされます。<br>フィルタリング項目は最大5件を指定することができます。<br>各フィルタリング項目間は論理積[AND]で結合されます。"));
	MessageProc::AddEng(CMDFRK_FILTER_DESC, _T("Filter settings can be specified here.<br>Records are filtered according to the specified criteria.<br>Up to five filter items can be specified.<br>All items are treated as logical AND operators."));

	//3128 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTER_INVALID, _T("設定されているフィルタリング条件は現在のテーブルのスキーマと一致していません。<br>フィルタリング条件をクリアしてよいですか？"));
	MessageProc::AddEng(CMDFRK_FILTER_INVALID, _T("The filter conditions set do not match the current table schema.<br>Do you want to clear the filter conditions?"));

	//3129 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTERINGCOND_UPDATE, _T("フィルタリング条件を更新しました。"));
	MessageProc::AddEng(CMDFRK_FILTERINGCOND_UPDATE, _T("The filter conditions have been updated."));

	//3130 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTERING_NOTABLE, _T("フィルタリングの対象となるテーブルが存在しないため操作を継続できません。"));
	MessageProc::AddEng(CMDFRK_FILTERING_NOTABLE, _T("The operation cannot be performed because the target table for filtering does not exist."));

	//3131 (NC6)
	MessageProc::AddJpn(CMDFRK_NOTABLEINFO, _T("表示対象のテーブルが存在しないため操作を継続できません。"));
	MessageProc::AddEng(CMDFRK_NOTABLEINFO, _T("The operation cannot be performed because the target table for display does not exist."));

	//3132 (NC6)
	MessageProc::AddJpn(CMDFRK_EMPTYCONNSTR, _T("本ソフトウェアを使用してデータベースのデータを参照するには，まずはじめにODBC接続の設定を行ってください。"));
	MessageProc::AddEng(CMDFRK_EMPTYCONNSTR, _T("To refer to data in a database using this software, the ODBC connection needs to be configured first."));

	//3133 (NC6)
	MessageProc::AddJpn(CMDFRK_WELCOME, _T("ようこそ，CmdFreakのページです！"));
	MessageProc::AddEng(CMDFRK_WELCOME, _T("Welcome to the CmdFreak page!"));

	//3134 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_TOOMUCHRECS, _T("対象テーブルのレコード数がCmdFreakで操作可能な上限を超えました。"));
	MessageProc::AddEng(CMDFRK_ERROR_TOOMUCHRECS, _T("The number of records in the target table exceeds the limit that CmdFreak can handle."));

	//3135 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_TOOLONGCONNSTR, _T("ODBCの接続文字列で指定可能な長さの上限を超えました。"));
	MessageProc::AddEng(CMDFRK_ERROR_TOOLONGCONNSTR, _T("The length of the ODBC connection string exceeds the allowable limit."));

	//3136 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_TOOLONGFILTER, _T("フィルタリングの条件が指定可能な上限を超えました。"));
	MessageProc::AddEng(CMDFRK_ERROR_TOOLONGFILTER, _T("The length of the filtering condition string exceeds the allowable limit."));

	//3142 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCFAILEDTOSTART, _T("CmdFreakサービスの開始に失敗しました。"));
	MessageProc::AddEng(CMDFRK_SVCFAILEDTOSTART, _T("CmdFreak service initiation failed."));

	//3143 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCFAILEDTOSTOP, _T("CmdFreakサービスの停止に失敗しました。"));
	MessageProc::AddEng(CMDFRK_SVCFAILEDTOSTOP, _T("CmdFreak service termination failed."));

	//3144 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCALREADYSTART, _T("CmdFreakサービスはすでに開始しています。"));
	MessageProc::AddEng(CMDFRK_SVCALREADYSTART, _T("CmdFreak service has already been started."));

	//3145 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCALREADYSTOP, _T("CmdFreakサービスはすでに停止しています。"));
	MessageProc::AddEng(CMDFRK_SVCALREADYSTOP, _T("CmdFreak service has already been stopped."));

	//3146 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTART, _T("CmdFreakサービスを開始しました。"));
	MessageProc::AddEng(CMDFRK_SVCSTART, _T("CmdFreak service has started."));

	//3147 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTOP, _T("CmdFreakサービスを停止しました。"));
	MessageProc::AddEng(CMDFRK_SVCSTOP, _T("CmdFreak service has stopped."));

	//3148 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DESC, _T("以下にトレースログが表示されます。<br>表示される時刻はWebサーバーが配置された地域の時刻となります。"));
	MessageProc::AddEng(CMDFRK_LOG_DESC, _T("The trace log is displayed below.<br>Times are as per the time zone of the web server\\'s location."));

	//3149 (NC6)
	MessageProc::AddJpn(CMDFRK_TABLEINFO, _T("以下に指定されたテーブルの詳細情報を表示します。"));
	MessageProc::AddEng(CMDFRK_TABLEINFO, _T("Detailed information on the specified table is shown below."));

	//3150 (---)
	MessageProc::AddJpn(CMDFRK_TARGETTABLE, _T("対象テーブル: "));
	MessageProc::AddEng(CMDFRK_TARGETTABLE, _T("The target table: "));

	//3154 (NC6)
	MessageProc::AddJpn(CMDFRK_COMPOINFO2, _T("このソフトウェアは次の技術を取り込んでいます。<br/>- Mongoose 3.7 (Released as MIT License)<br/>- JQuery 1.9.1 (Released as MIT License)<br/>- JQuery UI 1.10.3 (Released as MIT License)<br/>- ParamQuery Grid 1.1.2 (Released as MIT License)<br/>"));
	MessageProc::AddEng(CMDFRK_COMPOINFO2, _T("This software has the following technologies embedded:<br/>- Mongoose 3.7 (Released as MIT License)<br/>- JQuery 1.9.1 (Released as MIT License)<br/>- JQuery UI 1.10.3 (Released as MIT License)<br/>- ParamQuery Grid 1.1.2 (Released as MIT License)<br/>"));

	//3200 (---)
	MessageProc::AddJpn(CMDFRK_DBMS, _T("DBMSの選択 : "));
	MessageProc::AddEng(CMDFRK_DBMS, _T("Select DBMS : "));

	//3201 (---)
	MessageProc::AddJpn(CMDFRK_CONNSTR, _T("接続文字列の指定 : "));
	MessageProc::AddEng(CMDFRK_CONNSTR, _T("Specify connection string : "));

	//3202 (---)
	MessageProc::AddJpn(CMDFRK_MANUAL, _T("manual/jpn/index.htm"));
	MessageProc::AddEng(CMDFRK_MANUAL, _T("manual/eng/index.htm"));

	//3203 (---)
	MessageProc::AddJpn(CMDFRK_MANUALMSG, _T("オンラインマニュアルを開く"));
	MessageProc::AddEng(CMDFRK_MANUALMSG, _T("Open online manual"));

	//3210 (---)
	MessageProc::AddJpn(CMDFRK_COMPOINFO, _T("CmdFreak Version 1.0.0<br/>Copyright (C) 2014 Shinya Takeuchi<br/>All Rights Reserved."));
	MessageProc::AddEng(CMDFRK_COMPOINFO, _T("CmdFreak Version 1.0.0<br/>Copyright (C) 2014 Shinya Takeuchi<br/>All Rights Reserved."));

	//3226 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGTIME, _T("ログ出力時刻"));
	MessageProc::AddEng(CMDFRK_LOGGINGTIME, _T("Logging time"));

	//3227 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGMSG, _T("メッセージ"));
	MessageProc::AddEng(CMDFRK_LOGGINGMSG, _T("Message"));

	//3233 (---)
	MessageProc::AddJpn(CMDFRK_UNSPEC, _T("未指定"));
	MessageProc::AddEng(CMDFRK_UNSPEC, _T("Unspecified"));

	//3234 (---)
	MessageProc::AddJpn(CMDFRK_FILTERING, _T("フィルタリング"));
	MessageProc::AddEng(CMDFRK_FILTERING, _T("Filtering"));

	//3235 (---)
	MessageProc::AddJpn(CMDFRK_ODBCCONNECTIONS, _T("ODBC接続"));
	MessageProc::AddEng(CMDFRK_ODBCCONNECTIONS, _T("ODBC Connections"));

	//3236 (---)
	MessageProc::AddJpn(CMDFRK_TABLES, _T("テーブル情報"));
	MessageProc::AddEng(CMDFRK_TABLES, _T("Table Information"));

	//3237 (---)
	MessageProc::AddJpn(CMDFRK_CREATERECORD, _T("レコード生成"));
	MessageProc::AddEng(CMDFRK_CREATERECORD, _T("Create Record"));

	//3238 (---)
	MessageProc::AddJpn(CMDFRK_EDITRECORD, _T("レコード編集"));
	MessageProc::AddEng(CMDFRK_EDITRECORD, _T("Edit Record"));

	//3239 (---)
	MessageProc::AddJpn(CMDFRK_DELETERECORD, _T("レコード削除"));
	MessageProc::AddEng(CMDFRK_DELETERECORD, _T("Delete Record"));

	//3240 (---)
	MessageProc::AddJpn(CMDFRK_INFORMATION, _T("情報"));
	MessageProc::AddEng(CMDFRK_INFORMATION, _T("Information"));

	//3241 (---)
	MessageProc::AddJpn(CMDFRK_REFRESHTABLELIST, _T("テーブル一覧更新"));
	MessageProc::AddEng(CMDFRK_REFRESHTABLELIST, _T("Refresh Table List"));

}

void MyMsgProc::SetLocaleMode(int Mode)
{
	MessageProc::SetLocaleMode(Mode);
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
