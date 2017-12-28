#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "MyMsgProc.h"
#include "ApiGetRecord.h"
#include "dataaccess.h"
#include "..\Global.h"
#include "OdbcManager.h"
#include "DbAccessor.h"

StkObject* ApiGetRecord::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));
	if (StrStr(UrlPath, _T("?query="))) {
	}

	return NULL;
}
