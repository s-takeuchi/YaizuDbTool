#pragma once
#include "ApiBase.h"

class ApiGetLanguage : ApiBase
{
	StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
