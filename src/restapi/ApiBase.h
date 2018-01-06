#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class ApiBase : public StkWebAppExec
{
protected:
	void AddCodeAndMsg(StkObject*, int, TCHAR*, TCHAR*);
	void ApiBase::GetLocalTimeWStr(TCHAR[32]);
	void PrintRequest(int, TCHAR[128]);
	void PrintResponse(int);
	virtual StkObject* ExecuteImpl(StkObject*, int, TCHAR[128], int*, TCHAR[3]) = 0;

public:
	virtual StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
