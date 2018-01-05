#pragma once
#include "ApiBase.h"

class ApiOdbcInfo : public ApiBase
{
private:
	StkObject* GetOdbcInfo(TCHAR[128], int*);
	StkObject* PostOdbcInfo(StkObject*, int*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
