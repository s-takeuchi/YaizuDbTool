#pragma once
#include "ApiBase.h"

class ApiGetLanguage : public ApiBase
{
protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
