#pragma once
#include "ApiBase.h"

class ApiLogging : public ApiBase
{
	StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
