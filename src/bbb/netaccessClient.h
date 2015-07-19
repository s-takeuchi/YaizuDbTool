#pragma once
#include <windows.h>
#include "..\Global.h"

class NetAccessClient
{
private:
	// This instance
	static NetAccessClient* ThisInstance;
	NetAccessClient();
	~NetAccessClient();

public:
	// Get this instance
	static NetAccessClient* GetInstance();

	int TestConn(TCHAR*, int);
	int GetDfCnS(TCHAR*, int, char[20], TCHAR[Global::MAX_PARAM_LENGTH]);
	int GetConSt(TCHAR*, int, TCHAR[Global::MAX_PARAM_LENGTH]);
	int SetConSt(TCHAR*, int, int, TCHAR[Global::MAX_PARAM_LENGTH]);
	int GetSvInf(TCHAR*, int, char[32], char[32]);
	int GetNumOfRecs(TCHAR* Hostname, int Port, TCHAR TableNmae[Global::TABLENAME_LENGTH]);
	int GetRecrd(TCHAR*, int, TCHAR*, int, TCHAR[Global::TABLENAME_LENGTH]);
	int GetColms(TCHAR*, int, TCHAR[Global::MAX_PARAM_LENGTH], TCHAR[Global::TABLENAME_LENGTH]);
	int GetTblns(TCHAR*, int, TCHAR[Global::MAX_PARAM_LENGTH]);
	int LoggingI(TCHAR*, int, TCHAR[100]);
	int LogsHtml(TCHAR*, int, TCHAR[Global::MAX_PARAM_LENGTH / 2]);
	int FilterSw(TCHAR*, int, BOOL);
	BOOL GetFilSw(TCHAR*, int);
	int SetFilCd(TCHAR*, int, int, TCHAR[Global::COLUMNNAME_LENGTH], int, TCHAR[Global::COLUMNVAL_LENGTH]);
	int NetAccessClient::GetFilCd(TCHAR*, int, TCHAR[Global::MAX_PARAM_LENGTH]);
};
