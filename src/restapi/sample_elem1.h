#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class Sample_Elem1 : StkWebAppExec
{
private:
	TCHAR LocalTimeStr[32];
	void GetLocalTimeWStr();

public:
	Sample_Elem1();
	StkObject* Execute(StkObject*, int, TCHAR[128], int*);
};
