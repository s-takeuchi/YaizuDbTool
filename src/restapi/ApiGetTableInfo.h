﻿#pragma once
#include "ApiBase.h"

class ApiGetTableInfo : public ApiBase
{
protected:
	StkObject* ExecuteImpl(StkObject*, int, wchar_t[StkWebAppExec::URL_PATH_LENGTH], int*, wchar_t[3]);
};
