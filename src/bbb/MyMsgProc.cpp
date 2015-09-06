#include "..\..\..\YaizuComLib\src\\msgproc\msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================
	//2901 (NC2)
	MessageProc::AddJpn(UNKNOWNERROR, _T("�s���ȃG���[���������܂����B"));
	MessageProc::AddEng(UNKNOWNERROR, _T("An unknown error has occurred."));

	//2902 (NC2)
	MessageProc::AddJpn(FILEACCESSERROR, _T("�t�@�C���A�N�Z�X�G���[���������܂����B"));
	MessageProc::AddEng(FILEACCESSERROR, _T("A file access error has occurred."));

	//2903 (NC1)
	MessageProc::AddJpn(NEEDADMINRIGHTS, _T("���̃v���O���������s���邽�߂ɂ�Local Administrator�̌������K�v�ł��B\r\n�v���O�����͏I�����܂��B\r\n\r\n"));
	MessageProc::AddEng(NEEDADMINRIGHTS, _T("You need local administrator's rights to run this program.\r\nThe program will be terminated.\r\n\r\n"));

	// 2904 (NC3)
	MessageProc::AddJpn(DATAUPDATED, _T("�v���O�������g�p����f�[�^�͍X�V����Ă��܂��B\r\n�f�[�^�������܂�������𑱂��ėǂ��ł����B"));
	MessageProc::AddEng(DATAUPDATED, _T("Program data updated.\r\nData will be lost. Continue operation?"));

	// 2905 (NC4)
	MessageProc::AddJpn(INVALIDVALUE, _T("�w�肵���l���s���ł��B�l�̎w��\�Ȕ͈͂��m�F���Ă��������B"));
	MessageProc::AddEng(INVALIDVALUE, _T("Invalid value specified. Check the range of values allowed."));

	//2906 (---)
	MessageProc::AddJpn(AN_ERROR_OCCURRED, _T("�G���[���������܂����B"));
	MessageProc::AddEng(AN_ERROR_OCCURRED, _T("An error occurred."));

	//2931 (---)
	MessageProc::AddJpn(COMMON_ADD, _T("�ǉ�"));
	MessageProc::AddEng(COMMON_ADD, _T("Add"));

	//2932 (---)
	MessageProc::AddJpn(COMMON_DELETE, _T("�폜"));
	MessageProc::AddEng(COMMON_DELETE, _T("Delete"));

	//2933 (---)
	MessageProc::AddJpn(COMMON_EDIT, _T("�ҏW"));
	MessageProc::AddEng(COMMON_EDIT, _T("Edit"));

	//2934 (---)
	MessageProc::AddJpn(COMMON_REFRESH, _T("�X�V"));
	MessageProc::AddEng(COMMON_REFRESH, _T("Refresh"));

	//2935 (---)
	MessageProc::AddJpn(COMMON_TRUE, _T("True"));
	MessageProc::AddEng(COMMON_TRUE, _T("True"));

	//2936 (---)
	MessageProc::AddJpn(COMMON_FALSE, _T("False"));
	MessageProc::AddEng(COMMON_FALSE, _T("False"));

	//2937 (---)
	MessageProc::AddJpn(COMMON_UNKNOWN, _T("�s��"));
	MessageProc::AddEng(COMMON_UNKNOWN, _T("Unknown"));

	//2941 (---)
	MessageProc::AddJpn(COMMON_OK, _T("OK"));
	MessageProc::AddEng(COMMON_OK, _T("OK"));

	//2942 (---)
	MessageProc::AddJpn(COMMON_CANCEL, _T("��ݾ�"));
	MessageProc::AddEng(COMMON_CANCEL, _T("Cancel"));

	// =====================================================================================================
	//3100 (---)
	MessageProc::AddJpn(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2014 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));
	MessageProc::AddEng(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2014 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));

	//3101 (NC6)
	MessageProc::AddJpn(CMDFRK_CANNOT_START, _T("\r\n���̃v���O������Windows�T�[�r�X�v���O�����̂��ߋN�����邱�Ƃ��ł��܂���B\r\n���̃v���O������CmdFreak�̃C���X�g�[������Windows�T�[�r�X�Ƃ��ăV�X�e���ɓo�^����܂��B\r\n\r\n"));
	MessageProc::AddEng(CMDFRK_CANNOT_START, _T("\r\nThis program cannot be started because it is configured as a Windows service program.\r\nThe program is registered as a Windows service in the system when CmdFreak is installed.\r\n\r\n"));

	//3121 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_UNEXPECTED, _T("�\�����Ȃ��G���[���������܂����B���̃G���[��Web�u���E�U����̃��N�G�X�g����͂ł��Ȃ������Ƃ��ɔ�������ꍇ������܂��B<br>"));
	MessageProc::AddEng(CMDFRK_ERROR_UNEXPECTED, _T("An unexpected error occurred. This may be caused by inability to analyze request data from the web browser.<br>"));

	//3122 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_SERVICEDOWN, _T("CmdFreak�T�[�r�X�Ƃ̒ʐM�����s���܂����B���̗v�����l�����܂��B<br>(1) CmdFreak�T�[�r�X���J�n����Ă��Ȃ��B<br>(2) CmdFreak�T�[�r�X���t�@�C�A�E�H�[���ɗ�O�o�^����Ă��Ȃ��B<br>(3) �ڑ���z�X�g������у|�[�g�ԍ��̒�`�t�@�C�� [bbb.conf, cmdfrksrv.conf] ���s���B<br>(4) CmdFreak�̓��������ňُ킪�������Ă���B<br>"));
	MessageProc::AddEng(CMDFRK_ERROR_SERVICEDOWN, _T("Connection with CmdFreak service failed. This may be caused by one of the following issues:<br>(1) CmdFreak service cannot be started.<br>(2) CmdFreak service is not registered as a firewall exception.<br>(3) The definition file [bbb.conf and/or cmdfrksrv.conf] for the host name and port number in the network connectivity settings is invalid.<br>(4) A CmdFreak internal process is invalid.<br>"));

	//3123 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_JAVASCRIPTERR, _T("JavaScript�������ł��B<br>�u���E�U�̐ݒ��ύX����JavaScript��L����C�y�[�W�����t���b�V�����Ă��������B<br>\n"));
	MessageProc::AddEng(CMDFRK_ERROR_JAVASCRIPTERR, _T("JavaScript is disabled in your browser.<br>Change the browser setting to enable it and refresh the page.<br>\n"));

	//3124 (NC6)
	MessageProc::AddJpn(CMDFRK_ODBCINFO, _T("ODBC�ڑ��̐ݒ���s���܂��B<br>�ڑ��Ώۂ�DBMS��I�����CODBC�̐ڑ���������w�肵�Ă��������B<br>�{�\�t�g�E�F�A��32�r�b�g�A�v���P�[�V�����̂��߁CODBC�ڑ�������ɂ́C32�r�b�g��ODBC�h���C�o���g�p����悤�Ɏw����s���Ă��������B�I�y���[�e�B���O�V�X�e���ɂ́C���炩����32�r�b�g��ODBC�h���C�o���C���X�g�[������Ă���K�v������܂��B<br>"));
	MessageProc::AddEng(CMDFRK_ODBCINFO, _T("Configure ODBC connection.<br>Select the connection target DBMS and specify the ODBC connection string.<br>As this is 32-bit application software, this string should be specified for the use of a 32-bit ODBC driver. As a prerequisite, a 32-bit ODBC driver needs to be installed on the operating system.<br>"));

	//3125 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_INVALIDDBCONN, _T("DBMS�Ƃ̐ڑ��Ɏ��s���܂����B<br>���̗v�����l�����܂��B<br>- DBMS���K�؂ɋN�����Ă��Ȃ��B<br>- ODBC�h���C�o���C���X�g�[������Ă��Ȃ��B<br>- ODBC�̐ڑ������񂪕s���B<br>- DBMS�Ƃ̐ڑ��o�H�ɉ��炩�̖�肪����B<br>"));
	MessageProc::AddEng(CMDFRK_ERROR_INVALIDDBCONN, _T("DBMS connection failed.<br>This may be caused by one of the following issues:<br>- The DBMS is not working properly.<br>- No ODBC driver is installed.<br>- The ODBC connection string is invalid.<br>- There are issues with the DBMS on the network.<br>"));

	//3126 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DBMSCHANGE, _T("ODBC�ڑ��̐ݒ��ύX���܂����B"));
	MessageProc::AddEng(CMDFRK_LOG_DBMSCHANGE, _T("The ODBC connection configuration has been changed."));

	//3127 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTER_DESC, _T("�����ł̓t�B���^�����O�̐ݒ���s�����Ƃ��ł��܂��B<br>�w�肵�������Ń��R�[�h��񂪃t�B���^�����O����܂��B<br>�t�B���^�����O���ڂ͍ő�5�����w�肷�邱�Ƃ��ł��܂��B<br>�e�t�B���^�����O���ڊԂ͘_����[AND]�Ō�������܂��B"));
	MessageProc::AddEng(CMDFRK_FILTER_DESC, _T("Filter settings can be specified here.<br>Records are filtered according to the specified criteria.<br>Up to five filter items can be specified.<br>All items are treated as logical AND operators."));

	//3128 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTER_INVALID, _T("�ݒ肳��Ă���t�B���^�����O�����͌��݂̃e�[�u���̃X�L�[�}�ƈ�v���Ă��܂���B<br>�t�B���^�����O�������N���A���Ă悢�ł����H"));
	MessageProc::AddEng(CMDFRK_FILTER_INVALID, _T("The filter conditions set do not match the current table schema.<br>Do you want to clear the filter conditions?"));

	//3129 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTERINGCOND_UPDATE, _T("�t�B���^�����O�������X�V���܂����B"));
	MessageProc::AddEng(CMDFRK_FILTERINGCOND_UPDATE, _T("The filter conditions have been updated."));

	//3130 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTERING_NOTABLE, _T("�t�B���^�����O�̑ΏۂƂȂ�e�[�u�������݂��Ȃ����ߑ�����p���ł��܂���B"));
	MessageProc::AddEng(CMDFRK_FILTERING_NOTABLE, _T("The operation cannot be performed because the target table for filtering does not exist."));

	//3131 (NC6)
	MessageProc::AddJpn(CMDFRK_NOTABLEINFO, _T("�\���Ώۂ̃e�[�u�������݂��Ȃ����ߑ�����p���ł��܂���B"));
	MessageProc::AddEng(CMDFRK_NOTABLEINFO, _T("The operation cannot be performed because the target table for display does not exist."));

	//3132 (NC6)
	MessageProc::AddJpn(CMDFRK_EMPTYCONNSTR, _T("�{�\�t�g�E�F�A���g�p���ăf�[�^�x�[�X�̃f�[�^���Q�Ƃ���ɂ́C�܂��͂��߂�ODBC�ڑ��̐ݒ���s���Ă��������B"));
	MessageProc::AddEng(CMDFRK_EMPTYCONNSTR, _T("To refer to data in a database using this software, the ODBC connection needs to be configured first."));

	//3133 (NC6)
	MessageProc::AddJpn(CMDFRK_WELCOME, _T("�悤�����CCmdFreak�̃y�[�W�ł��I"));
	MessageProc::AddEng(CMDFRK_WELCOME, _T("Welcome to the CmdFreak page!"));

	//3134 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_TOOMUCHRECS, _T("�Ώۃe�[�u���̃��R�[�h����CmdFreak�ő���\�ȏ���𒴂��܂����B"));
	MessageProc::AddEng(CMDFRK_ERROR_TOOMUCHRECS, _T("The number of records in the target table exceeds the limit that CmdFreak can handle."));

	//3135 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_TOOLONGCONNSTR, _T("ODBC�̐ڑ�������Ŏw��\�Ȓ����̏���𒴂��܂����B"));
	MessageProc::AddEng(CMDFRK_ERROR_TOOLONGCONNSTR, _T("The length of the ODBC connection string exceeds the allowable limit."));

	//3136 (NC6)
	MessageProc::AddJpn(CMDFRK_ERROR_TOOLONGFILTER, _T("�t�B���^�����O�̏������w��\�ȏ���𒴂��܂����B"));
	MessageProc::AddEng(CMDFRK_ERROR_TOOLONGFILTER, _T("The length of the filtering condition string exceeds the allowable limit."));

	//3142 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCFAILEDTOSTART, _T("CmdFreak�T�[�r�X�̊J�n�Ɏ��s���܂����B"));
	MessageProc::AddEng(CMDFRK_SVCFAILEDTOSTART, _T("CmdFreak service initiation failed."));

	//3143 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCFAILEDTOSTOP, _T("CmdFreak�T�[�r�X�̒�~�Ɏ��s���܂����B"));
	MessageProc::AddEng(CMDFRK_SVCFAILEDTOSTOP, _T("CmdFreak service termination failed."));

	//3144 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCALREADYSTART, _T("CmdFreak�T�[�r�X�͂��łɊJ�n���Ă��܂��B"));
	MessageProc::AddEng(CMDFRK_SVCALREADYSTART, _T("CmdFreak service has already been started."));

	//3145 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCALREADYSTOP, _T("CmdFreak�T�[�r�X�͂��łɒ�~���Ă��܂��B"));
	MessageProc::AddEng(CMDFRK_SVCALREADYSTOP, _T("CmdFreak service has already been stopped."));

	//3146 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTART, _T("CmdFreak�T�[�r�X���J�n���܂����B"));
	MessageProc::AddEng(CMDFRK_SVCSTART, _T("CmdFreak service has started."));

	//3147 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTOP, _T("CmdFreak�T�[�r�X���~���܂����B"));
	MessageProc::AddEng(CMDFRK_SVCSTOP, _T("CmdFreak service has stopped."));

	//3148 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DESC, _T("�ȉ��Ƀg���[�X���O���\������܂��B<br>�\������鎞����Web�T�[�o�[���z�u���ꂽ�n��̎����ƂȂ�܂��B"));
	MessageProc::AddEng(CMDFRK_LOG_DESC, _T("The trace log is displayed below.<br>Times are as per the time zone of the web server\\'s location."));

	//3149 (NC6)
	MessageProc::AddJpn(CMDFRK_TABLEINFO, _T("�ȉ��Ɏw�肳�ꂽ�e�[�u���̏ڍ׏���\�����܂��B"));
	MessageProc::AddEng(CMDFRK_TABLEINFO, _T("Detailed information on the specified table is shown below."));

	//3150 (---)
	MessageProc::AddJpn(CMDFRK_TARGETTABLE, _T("�Ώۃe�[�u��: "));
	MessageProc::AddEng(CMDFRK_TARGETTABLE, _T("The target table: "));

	//3154 (NC6)
	MessageProc::AddJpn(CMDFRK_COMPOINFO2, _T("���̃\�t�g�E�F�A�͎��̋Z�p����荞��ł��܂��B<br/>- Mongoose 3.7 (Released as MIT License)<br/>- JQuery 1.9.1 (Released as MIT License)<br/>- JQuery UI 1.10.3 (Released as MIT License)<br/>- ParamQuery Grid 1.1.2 (Released as MIT License)<br/>"));
	MessageProc::AddEng(CMDFRK_COMPOINFO2, _T("This software has the following technologies embedded:<br/>- Mongoose 3.7 (Released as MIT License)<br/>- JQuery 1.9.1 (Released as MIT License)<br/>- JQuery UI 1.10.3 (Released as MIT License)<br/>- ParamQuery Grid 1.1.2 (Released as MIT License)<br/>"));

	//3200 (---)
	MessageProc::AddJpn(CMDFRK_DBMS, _T("DBMS�̑I�� : "));
	MessageProc::AddEng(CMDFRK_DBMS, _T("Select DBMS : "));

	//3201 (---)
	MessageProc::AddJpn(CMDFRK_CONNSTR, _T("�ڑ�������̎w�� : "));
	MessageProc::AddEng(CMDFRK_CONNSTR, _T("Specify connection string : "));

	//3202 (---)
	MessageProc::AddJpn(CMDFRK_MANUAL, _T("manual/jpn/index.htm"));
	MessageProc::AddEng(CMDFRK_MANUAL, _T("manual/eng/index.htm"));

	//3203 (---)
	MessageProc::AddJpn(CMDFRK_MANUALMSG, _T("�I�����C���}�j���A�����J��"));
	MessageProc::AddEng(CMDFRK_MANUALMSG, _T("Open online manual"));

	//3210 (---)
	MessageProc::AddJpn(CMDFRK_COMPOINFO, _T("CmdFreak Version 1.0.0<br/>Copyright (C) 2014 Shinya Takeuchi<br/>All Rights Reserved."));
	MessageProc::AddEng(CMDFRK_COMPOINFO, _T("CmdFreak Version 1.0.0<br/>Copyright (C) 2014 Shinya Takeuchi<br/>All Rights Reserved."));

	//3226 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGTIME, _T("���O�o�͎���"));
	MessageProc::AddEng(CMDFRK_LOGGINGTIME, _T("Logging time"));

	//3227 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGMSG, _T("���b�Z�[�W"));
	MessageProc::AddEng(CMDFRK_LOGGINGMSG, _T("Message"));

	//3233 (---)
	MessageProc::AddJpn(CMDFRK_UNSPEC, _T("���w��"));
	MessageProc::AddEng(CMDFRK_UNSPEC, _T("Unspecified"));

	//3234 (---)
	MessageProc::AddJpn(CMDFRK_FILTERING, _T("�t�B���^�����O"));
	MessageProc::AddEng(CMDFRK_FILTERING, _T("Filtering"));

	//3235 (---)
	MessageProc::AddJpn(CMDFRK_ODBCCONNECTIONS, _T("ODBC�ڑ�"));
	MessageProc::AddEng(CMDFRK_ODBCCONNECTIONS, _T("ODBC Connections"));

	//3236 (---)
	MessageProc::AddJpn(CMDFRK_TABLES, _T("�e�[�u�����"));
	MessageProc::AddEng(CMDFRK_TABLES, _T("Table Information"));

	//3237 (---)
	MessageProc::AddJpn(CMDFRK_CREATERECORD, _T("���R�[�h����"));
	MessageProc::AddEng(CMDFRK_CREATERECORD, _T("Create Record"));

	//3238 (---)
	MessageProc::AddJpn(CMDFRK_EDITRECORD, _T("���R�[�h�ҏW"));
	MessageProc::AddEng(CMDFRK_EDITRECORD, _T("Edit Record"));

	//3239 (---)
	MessageProc::AddJpn(CMDFRK_DELETERECORD, _T("���R�[�h�폜"));
	MessageProc::AddEng(CMDFRK_DELETERECORD, _T("Delete Record"));

	//3240 (---)
	MessageProc::AddJpn(CMDFRK_INFORMATION, _T("���"));
	MessageProc::AddEng(CMDFRK_INFORMATION, _T("Information"));

	//3241 (---)
	MessageProc::AddJpn(CMDFRK_REFRESHTABLELIST, _T("�e�[�u���ꗗ�X�V"));
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
