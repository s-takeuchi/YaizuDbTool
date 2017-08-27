#pragma once
#include "ApiBase.h"

class ApiGetLanguage : public ApiBase
{
	StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
