#pragma once
#include "..\..\..\YaizuComLib\src\stkwebapp\StkWebAppExec.h"

class ApiBase : public StkWebAppExec
{
protected:
	void AddCodeAndMsg(StkObject*, int, TCHAR*, TCHAR*);
};
