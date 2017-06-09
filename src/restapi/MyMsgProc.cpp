#include "..\..\..\YaizuComLib\src\\commonfunc\msgproc.h"
#include "MyMsgProc.h"

void MyMsgProc::AddMsg()
{
	// =====================================================================================================
	//3100 (---)
	MessageProc::AddJpn(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2017 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));
	MessageProc::AddEng(CMDFRK_CMDFRKSRV, _T("CmdFreak service program\r\nCopyright (C) 2017 Shinya Takeuchi\r\nAll Rights Reserved.\r\n"));

	//3111 (��)
	MessageProc::AddJpn(CMDFRK_CONF_NOT_FOUND, _T("stkwebapp.conf ��������܂���B\r\n"));
	MessageProc::AddEng(CMDFRK_CONF_NOT_FOUND, _T("stkwebapp.conf does not found.\r\n"));

	//3112 (��)
	MessageProc::AddJpn(CMDFRK_DAT_NOT_FOUND, _T("stkwebapp.dat ��������܂���B\r\n"));
	MessageProc::AddEng(CMDFRK_DAT_NOT_FOUND, _T("stkwebapp.dat does not found.\r\n"));

	//3113 (��)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, _T("servicehost ��stkwebapp.conf�̒��Ɍ�����܂���B\r\n"));
	MessageProc::AddEng(CMDFRK_DAT_SERVICEHOST_NOT_FOUND, _T("servicehost does not found in stkwebapp.conf.\r\n"));

	//3114 (��)
	MessageProc::AddJpn(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, _T("serviceport ��stkwebapp.conf�̒��Ɍ�����܂���B\r\n"));
	MessageProc::AddEng(CMDFRK_DAT_SERVICEPORT_NOT_FOUND, _T("serviceport does not found in stkwebapp.conf.\r\n"));

	//3120 (��)
	MessageProc::AddJpn(CMDFRK_NO_CLIENTLOCALE, _T("�N���C�A���g�̃��P�[�������m�ł��܂���B"));
	MessageProc::AddEng(CMDFRK_NO_CLIENTLOCALE, _T("Client locale cannot be detected."));

	//3126 (NC6)
	MessageProc::AddJpn(CMDFRK_LOG_DBMSCHANGE, _T("ODBC�ڑ��̐ݒ��ύX���܂����B"));
	MessageProc::AddEng(CMDFRK_LOG_DBMSCHANGE, _T("The ODBC connection configuration has been changed."));

	//3129 (NC6)
	MessageProc::AddJpn(CMDFRK_FILTERINGCOND_UPDATE, _T("�t�B���^�����O�������X�V���܂����B"));
	MessageProc::AddEng(CMDFRK_FILTERINGCOND_UPDATE, _T("The filter conditions have been updated."));

	//3146 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTART, _T("CmdFreak�T�[�r�X���J�n���܂����B"));
	MessageProc::AddEng(CMDFRK_SVCSTART, _T("CmdFreak service has started."));

	//3147 (NC6)
	MessageProc::AddJpn(CMDFRK_SVCSTOP, _T("CmdFreak�T�[�r�X���~���܂����B"));
	MessageProc::AddEng(CMDFRK_SVCSTOP, _T("CmdFreak service has stopped."));

	//3226 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGTIME, _T("���O�o�͎���"));
	MessageProc::AddEng(CMDFRK_LOGGINGTIME, _T("Logging time"));

	//3227 (---)
	MessageProc::AddJpn(CMDFRK_LOGGINGMSG, _T("���b�Z�[�W"));
	MessageProc::AddEng(CMDFRK_LOGGINGMSG, _T("Message"));

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