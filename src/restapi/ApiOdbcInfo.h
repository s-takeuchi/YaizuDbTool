#pragma once
#include "ApiBase.h"

class ApiOdbcInfo : public ApiBase
{
private:
	StkObject* GetOdbcInfo(TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*);
	StkObject* PostOdbcInfo(StkObject*, int*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*, TCHAR[3]);
};
