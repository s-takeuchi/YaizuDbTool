#pragma once
#include "ApiBase.h"

class ApiFilterInfo : public ApiBase
{
private:
	StkObject* GetFilterInfo(TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*);
	StkObject* PostFilterInfo(StkObject*, int*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*, TCHAR[3]);
};
