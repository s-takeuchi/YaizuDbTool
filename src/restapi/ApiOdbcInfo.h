#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiOdbcInfo : public ApiBase
{
private:
	StkObject* GetOdbcInfo(wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t*);
	StkObject* PostOdbcInfo(StkObject*, int*, wchar_t*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
