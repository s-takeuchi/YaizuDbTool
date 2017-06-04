#include <tchar.h>
#include "ApiGetLanguage.h"

StkObject* ApiGetLanguage::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	int ErrCode;
	StkObject* TmpObj = StkObject::CreateObjectFromJson(_T("{ \"hello\" : \"hello, world!!\" }"), &ErrCode);
	*ResultCode = 200;
	return TmpObj;
}
