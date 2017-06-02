#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class ApiBase : StkWebAppExec
{
protected:
	void AddCodeAndMsg(StkObject*, int, TCHAR*, TCHAR*);
};
