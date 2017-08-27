#pragma once
#include "ApiBase.h"

class ApiOdbcInfos : public ApiBase
{
private:
	StkObject* GetOdbcInfos(TCHAR[128], int*);
	void PostOdbcInfos(StkObject*, int*);

public:
	StkObject* Execute(StkObject*, int, TCHAR[128], int*, TCHAR[3]);
};
