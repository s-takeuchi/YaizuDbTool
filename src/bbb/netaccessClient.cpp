#include "..\..\..\YaizuComLib\src\stksocket\stksocket.h"
#include <windows.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "netaccessClient.h"
#include "..\Global.h"

NetAccessClient* NetAccessClient::ThisInstance;

NetAccessClient::NetAccessClient()
{
}

NetAccessClient::~NetAccessClient()
{
}

// Get this instance
NetAccessClient* NetAccessClient::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new NetAccessClient();
		Init = 0;
	}
	return ThisInstance;
}

int NetAccessClient::TestConn(TCHAR* Hostname, int Port)
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[TestConn]%09d ", 0);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, 20);
	StkSocket_Receive(1, 1, (BYTE*)Buf, 2, 10000002, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	Buf[2] = '\0';
	if (strcmp(Buf, "OK") == 0) {
		return 0;
	} else if (strcmp(Buf, "NG") == 0) {
		return -2;
	} else if (strcmp(Buf, "EM") == 0) {
		return -3;
	}else {
		return -1;
	}
}

int NetAccessClient::GetDfCnS(TCHAR* Hostname, int Port, char DbmsType[20], TCHAR ConnStr[Global::MAX_PARAM_LENGTH])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetDfCnS]%09d ", 20);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, 20);
	StkSocket_Send(1, 1, (BYTE*)DbmsType, 20);
	StkSocket_Receive(1, 1, (BYTE*)ConnStr, Global::MAX_PARAM_LENGTH * sizeof(TCHAR), 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::GetConSt(TCHAR* Hostname, int Port, TCHAR ConnStr[Global::MAX_PARAM_LENGTH])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetConSt]%09d ", 0);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Receive(1, 1, (BYTE*)ConnStr, Global::MAX_PARAM_LENGTH * sizeof(TCHAR), 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::SetConSt(TCHAR* Hostname, int Port, int DbmsType, TCHAR ConnStr[Global::MAX_PARAM_LENGTH])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[SetConSt]%09d ", (lstrlen(ConnStr) + 1) * sizeof(TCHAR));
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Send(1, 1, (BYTE*)ConnStr, (lstrlen(ConnStr) + 1) * sizeof(TCHAR));
	StkSocket_Receive(1, 1, (BYTE*)ConnStr, Global::MAX_PARAM_LENGTH * sizeof(TCHAR), 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::GetSvInf(TCHAR* Hostname, int Port, char BuildDate[32], char ServiceStartTime[32])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetSvInf]%09d ", 0);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, 20);
	StkSocket_Receive(1, 1, (BYTE*)BuildDate, 32, 10000032, (BYTE*)NULL, 0, FALSE);
	StkSocket_Receive(1, 1, (BYTE*)ServiceStartTime, 32, 10000032, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::GetNumOfRecs(TCHAR* Hostname, int Port, TCHAR TableNmae[Global::TABLENAME_LENGTH])
{
	char Buf[21];
	INT32 NumOfRecs = 0;
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetNumOR]%09d ", lstrlen(TableNmae) * sizeof(TCHAR));
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Send(1, 1, (BYTE*)TableNmae, lstrlen(TableNmae) * sizeof(TCHAR));
	StkSocket_Receive(1, 1, (BYTE*)&NumOfRecs, 4, 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return (int)NumOfRecs;
}

int NetAccessClient::GetRecrd(TCHAR* Hostname, int Port, TCHAR* Msg, int Length, TCHAR TableNmae[Global::TABLENAME_LENGTH])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetRecrd]%09d ", lstrlen(TableNmae) * sizeof(TCHAR));
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Send(1, 1, (BYTE*)TableNmae, lstrlen(TableNmae) * sizeof(TCHAR));
	StkSocket_Receive(1, 1, (BYTE*)Msg, Length * sizeof(TCHAR), 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::GetColms(TCHAR* Hostname, int Port, TCHAR Msg[Global::MAX_PARAM_LENGTH], TCHAR TableNmae[Global::TABLENAME_LENGTH])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetColms]%09d ", lstrlen(TableNmae) * sizeof(TCHAR));
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Send(1, 1, (BYTE*)TableNmae, lstrlen(TableNmae) * sizeof(TCHAR));
	StkSocket_Receive(1, 1, (BYTE*)Msg, Global::MAX_PARAM_LENGTH * sizeof(TCHAR), 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::GetTblns(TCHAR* Hostname, int Port, TCHAR Msg[Global::MAX_PARAM_LENGTH])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetTblns]%09d ", 0);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Receive(1, 1, (BYTE*)Msg, Global::MAX_PARAM_LENGTH * sizeof(TCHAR), 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::LoggingI(TCHAR* Hostname, int Port, TCHAR Msg[100])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[LoggingI]%09d ", 100 * sizeof(TCHAR));
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Send(1, 1, (BYTE*)Msg, 100 * sizeof(TCHAR));
	StkSocket_Receive(1, 1, (BYTE*)Buf, 2, 10000002, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::LogsHtml(TCHAR* Hostname, int Port, TCHAR LogAsHtml[Global::MAX_PARAM_LENGTH / 2])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[LogsHtml]%09d ", 0);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Receive(1, 1, (BYTE*)LogAsHtml, Global::MAX_PARAM_LENGTH, 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::FilterSw(TCHAR* Hostname, int Port, BOOL Switch)
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[FilterSw]%09d ", 1);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	if (Switch == TRUE) {
		StkSocket_Send(1, 1, (BYTE*)"1", 1);
	} else {
		StkSocket_Send(1, 1, (BYTE*)"0", 1);
	}
	StkSocket_Receive(1, 1, (BYTE*)Buf, 20, 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

BOOL NetAccessClient::GetFilSw(TCHAR* Hostname, int Port)
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetFilSw]%09d ", 0);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Receive(1, 1, (BYTE*)Buf, 20, 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	if (strstr(Buf, "TRUE") != NULL) {
		return TRUE;
	} else {
		return FALSE;
	}
}

int NetAccessClient::SetFilCd(TCHAR* Hostname, int Port, int Index, TCHAR ColumnName[Global::COLUMNNAME_LENGTH], int OpeType, TCHAR Value[Global::COLUMNVAL_LENGTH])
{
	char Buf[21];
	char First4Params[256];
	int LenOfColumnName = (lstrlen(ColumnName) + 1) * sizeof(TCHAR);
	int LenOfValue = (lstrlen(Value) + 1) * sizeof(TCHAR);
	sprintf_s(First4Params, "%d %d %d %d", Index, LenOfColumnName, OpeType, LenOfValue);
	int LenOfFirst4Params = strlen(First4Params) + 1;
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[SetFilCd]%09d ", LenOfFirst4Params + LenOfColumnName + LenOfValue);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Send(1, 1, (BYTE*)First4Params, LenOfFirst4Params);
	StkSocket_Send(1, 1, (BYTE*)ColumnName, LenOfColumnName);
	StkSocket_Send(1, 1, (BYTE*)Value, LenOfValue);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}

int NetAccessClient::GetFilCd(TCHAR* Hostname, int Port, TCHAR Msg[Global::MAX_PARAM_LENGTH])
{
	char Buf[21];
	StkSocket_AddInfo(1, STKSOCKET_TYPE_STREAM, STKSOCKET_ACTIONTYPE_SENDER, Hostname, Port);
	sprintf_s(Buf, "[GetFilCd]%09d ", 0);
	StkSocket_Connect(1);
	StkSocket_Send(1, 1, (BYTE*)Buf, strlen(Buf));
	StkSocket_Receive(1, 1, (BYTE*)Msg, Global::MAX_PARAM_LENGTH * sizeof(TCHAR), 9999999, (BYTE*)NULL, 0, FALSE);
	StkSocket_Disconnect(1, 1, TRUE);
	StkSocket_DeleteInfo(1);
	return 0;
}
