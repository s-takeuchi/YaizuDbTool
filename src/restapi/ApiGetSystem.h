#pragma once
#include "ApiBase.h"

class ApiGetSystem : public ApiBase
{
private:
	TCHAR LocalTimeStr[32];
	int NumOfThreads;

	void GetLocalTimeWStr();

public:
	ApiGetSystem();
	void SetNumOfThreads(int);

protected:
	StkObject* ExecuteImpl(StkObject*, int, TCHAR[StkWebAppExec::URL_PATH_LENGTH], int*, TCHAR[3]);
};
