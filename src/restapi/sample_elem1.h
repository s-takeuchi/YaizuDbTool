#pragma once
#include "ApiBase.h"

class Sample_Elem1 : ApiBase
{
private:
	TCHAR LocalTimeStr[32];
	int NumOfThreads;

	void GetLocalTimeWStr();

public:
	Sample_Elem1();
	void SetNumOfThreads(int);
	StkObject* Execute(StkObject*, int, TCHAR[128], int*);
};
