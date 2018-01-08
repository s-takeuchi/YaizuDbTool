#pragma once
#include "ApiBase.h"

class ApiFilterInfo : public ApiBase
{
protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
