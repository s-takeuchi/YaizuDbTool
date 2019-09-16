#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class ApiBase : public StkWebAppExec
{
protected:
	void AddCodeAndMsg(StkObject*, int, wchar_t*, wchar_t*);
	void PrintRequest(int, wchar_t[StkWebAppExec::URL_PATH_LENGTH]);
	void PrintResponse(int);
	void DecodeURL(wchar_t[StkWebAppExec::URL_PATH_LENGTH], wchar_t[StkWebAppExec::URL_PATH_LENGTH]);
	virtual StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3]) = 0;

public:
	virtual StkObject* Execute(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t*);
};
