#pragma once
#include "ApiBase.h"

class ApiGetSystem : public ApiBase
{
private:
	wchar_t LocalTimeStr[32];
	int NumOfThreads;

	void GetLocalTimeWStr();

public:
	ApiGetSystem();
	void SetNumOfThreads(int);

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
