#pragma once
#include "ApiBase.h"

class ApiOdbcInfo : public ApiBase
{
private:
	StkObject* GetOdbcInfo(wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*);
	StkObject* PostOdbcInfo(StkObject*, int*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3]);
};
