#include "..\..\..\YaizuComLib\src\\commonfunc\msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================
	//3100 (---)
	MessageProc::AddJpn(CMDFRK_CMDFRKSRV, L"CmdFreak service program\r\nCopyright (C) 2017 Shinya Takeuchi\r\nAll Rights Reserved.\r\n");
	MessageProc::AddEng(CMDFRK_CMDFRKSRV, L"CmdFreak service program\r\nCopyright (C) 2017 Shinya Takeuchi\r\nAll Rights Reserved.\r\n");

	//3111 (��)
	MessageProc::AddJpn(CMDFRK_CONF_NOT_FOUND, L"stkwebapp.conf ��������܂���B\r\n");
	MessageProc::AddEng(CMDFRK_CONF_NOT_FOUND, L"stkwebapp.conf does not found.\r\n");

	//3112 (��)
	MessageProc::AddJpn(CMDFRK_DAT_NOT_FOUND, L"sample.dat ��������܂���B\r\n");
	MessageProc::AddEng(CMDFRK_DAT_NOT_FOUND, L"sample.dat does not found.\r\n");

	//3113 (��)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, L"servicehost ��stkwebapp.conf�̒��Ɍ�����܂���B\r\n");
	MessageProc::AddEng(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, L"servicehost does not found in stkwebapp.conf.\r\n");

	//3114 (��)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, L"serviceport ��stkwebapp.conf�̒��Ɍ�����܂���B\r\n");
	MessageProc::AddEng(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, L"serviceport does not found in stkwebapp.conf.\r\n");

	//3120 (��)
	MessageProc::AddJpn(CMDFRK_NO_CLIENTLOCALE, L"�N���C�A���g�̃��P�[�������m�ł��܂���B");
	MessageProc::AddEng(CMDFRK_NO_CLIENTLOCALE, L"Client locale cannot be detected.");

	//3121 (��)
	MessageProc::AddJpn(CMDFRK_REQ_NOT_SUFFICIENT, L"�K�v�ȃp�����[�^�����N�G�X�g�Ɋ܂܂�Ă��܂���B");
	MessageProc::AddEng(CMDFRK_REQ_NOT_SUFFICIENT, L"The necessary parameter(s) is/are not contained in the request.");

	//3122 (��)
	MessageProc::AddJpn(CMDFRK_TABLE_NOT_EXIST, L"�w�肵���e�[�u�������݂��܂���B");
	MessageProc::AddEng(CMDFRK_TABLE_NOT_EXIST, L"The specified table does not exist.");

	//3123 (��)
	MessageProc::AddJpn(CMDFRK_PARAM_LENGTH_TOO_LONG, L"�w�肳�ꂽ�p�����[�^�́C�w��\�Ȓ����̍ő�𒴂��Ă��܂��B");
	MessageProc::AddEng(CMDFRK_PARAM_LENGTH_TOO_LONG, L"The specified parameter exceeds the maximum length.");

	//3126 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DBMSCHANGE, L"ODBC�ڑ��̐ݒ��ύX���܂����B");
	MessageProc::AddEng(CMDFRK_LOG_DBMSCHANGE, L"The ODBC connection configuration has been changed.");

	//3129 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_FILTERCHANGE, L"�t�B���^�����O�������X�V���܂����B");
	MessageProc::AddEng(CMDFRK_LOG_FILTERCHANGE, L"The filter criteria have been updated.");

	//3146 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTART, L"CmdFreak�T�[�r�X���J�n���܂����B");
	MessageProc::AddEng(CMDFRK_SVCSTART, L"CmdFreak service has started.");

	//3147 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTOP, L"CmdFreak�T�[�r�X���~���܂����B");
	MessageProc::AddEng(CMDFRK_SVCSTOP, L"CmdFreak service has stopped.");

	//3226 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGTIME, L"���O�o�͎���");
	MessageProc::AddEng(CMDFRK_LOGGINGTIME, L"Logging time");

	//3227 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGMSG, L"���b�Z�[�W");
	MessageProc::AddEng(CMDFRK_LOGGINGMSG, L"Message");

}

TCHAR* MyMsgProc::GetMsg(int Id)
{
	return MessageProc::GetMsg(Id);
}

char* MyMsgProc::GetMsgSjis(int Id)
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
