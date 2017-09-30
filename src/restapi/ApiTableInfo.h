#pragma once
#include "ApiBase.h"

class ApiTableInfo : public ApiBase
{
	StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
