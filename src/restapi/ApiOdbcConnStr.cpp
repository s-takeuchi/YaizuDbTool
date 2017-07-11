#include <tchar.h>
#include "ApiOdbcConnStr.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiOdbcConnStr::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));

	*ResultCode = 200;

	return ResObj;
}
