#pragma once
#include "ApiBase.h"

class ApiGetRecord : public ApiBase
{
protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*, TCHAR[3]);
};
