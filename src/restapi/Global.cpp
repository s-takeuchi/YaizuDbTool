#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "../../../YaizuComLib/src/commonfunc/msgproc.h"
#include "Global.h"
#include "MyMsgProc.h"

void Global::EventLogging(wchar_t* MsgEn, wchar_t* MsgJa, int UserId)
{
	StkWebAppUm_AddLogMsg(MsgEn, MsgJa, UserId);
	// Trace logging
	char TraceLogBuf[2048] = "";
	char* TraceLogTmp = StkPlCreateUtf8FromWideChar(MsgEn);
	StkPlSPrintf(TraceLogBuf, 1024, "Event : %s (UserId=%d)", TraceLogTmp, UserId);
	delete TraceLogTmp;
	MessageProc::AddLog(TraceLogBuf, MessageProc::LOG_TYPE_INFO);
}
