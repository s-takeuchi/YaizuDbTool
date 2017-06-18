#include <tchar.h>
#include "ApiLogging.h"

StkObject* ApiLogging::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	int ErrCode;
	StkObject* TmpObj = StkObject::CreateObjectFromJson(_T("{ \"hello2\" : \"hello, world2!!\" }"), &ErrCode);
	*ResultCode = 200;
	return TmpObj;
}
