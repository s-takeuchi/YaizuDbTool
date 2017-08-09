#pragma once
#include "ApiBase.h"

class ApiOdbcInfos : ApiBase
{
	StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
