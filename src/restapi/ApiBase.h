#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class ApiBase : public StkWebAppExec
{
protected:
	void AddCodeAndMsg(StkObject*, int, TCHAR*, TCHAR*);
	void ApiBase::GetLocalTimeWStr(TCHAR[32]);
	void PrintRequest(int, TCHAR[StkWebAppExec::URL_PATH_LENGTH]);
	void PrintResponse(int);
	void DecodeURL(TCHAR[StkWebAppExec::URL_PATH_LENGTH], TCHAR[StkWebAppExec::URL_PATH_LENGTH]);
	virtual StkObject* ExecuteImpl(StkObject*, int, TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*, TCHAR[3]) = 0;

public:
	virtual StkObject* Execute(StkObject*, int, TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*, TCHAR[3]);
};
