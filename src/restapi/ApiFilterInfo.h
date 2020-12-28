#pragma once
#include "../../../YaizuComLib/src/stkwebapp_um/ApiBase.h"

class ApiFilterInfo : public ApiBase
{
private:
	StkObject* GetFilterInfo(wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*);
	StkObject* PostFilterInfo(StkObject*, int*);

protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3], wchar_t*);
};
