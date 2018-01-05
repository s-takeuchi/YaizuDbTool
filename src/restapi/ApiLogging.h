#pragma once
#include "ApiBase.h"

class ApiLogging : public ApiBase
{
protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
