#pragma once
#include "ApiBase.h"

class ApiFilterInfo : public ApiBase
{
private:
	StkObject* GetFilterInfo(TCHAR[128], int*);
	StkObject* PostFilterInfo(StkObject*, int*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
