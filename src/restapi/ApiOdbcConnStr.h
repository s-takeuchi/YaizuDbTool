#pragma once
#include "ApiBase.h"

class ApiOdbcConnStr : ApiBase
{
	StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
