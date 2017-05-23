#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class Sample_Elem1 : StkWebAppExec
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
