#pragma once
#include "ApiBase.h"

class ApiGetSystem : ApiBase
{
private:
	TCHAR LocalTimeStr[32];
	int NumOfThreads;

	void GetLocalTimeWStr();

public:
	ApiGetSystem();
	void SetNumOfThreads(int);
	StkObject* Execute(StkObject*, int, TCHAR[128], int*);
};
