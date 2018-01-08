#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "MyMsgProc.h"
#include "ApiFilterInfo.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiFilterInfo::GetFilterInfo(TCHAR UrlPath[128], int* ResultCode)
{
	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));
	StkObject* FilInfObj = new StkObject(_T("FilterInfo"));
	DatObj->AppendChildElement(FilInfObj);

	BOOL FilterSw = DataAccess::GetInstance()->GetFilterSwitch();
	if (FilterSw) {
		FilInfObj->AppendChildElement(new StkObject(_T("Function"), _T("enable")));
	} else {
		FilInfObj->AppendChildElement(new StkObject(_T("Function"), _T("disable")));
	}
	for (int Loop = 1; Loop <= 5; Loop++) {
		TCHAR ColumnName[Global::COLUMNNAME_LENGTH];
		int FilterOpeType;
		TCHAR Value[Global::COLUMNVAL_LENGTH];

		DataAccess::GetInstance()->GetFilterCondition(Loop, ColumnName, &FilterOpeType, Value);
		StkObject* CriteriaObj = new StkObject(_T("Criteria"));
		CriteriaObj->AppendChildElement(new StkObject(_T("index"), Loop));
		CriteriaObj->AppendChildElement(new StkObject(_T("columnname"), ColumnName));
		CriteriaObj->AppendChildElement(new StkObject(_T("opetype"), FilterOpeType));
		CriteriaObj->AppendChildElement(new StkObject(_T("value"), Value));
		FilInfObj->AppendChildElement(CriteriaObj);
	}

	ResObj->AppendChildElement(DatObj);
	*ResultCode = 200;
	return ResObj;
}

StkObject* ApiFilterInfo::PostFilterInfo(StkObject* ReqObj, int* ResultCode)
{
	return NULL;
}

StkObject* ApiFilterInfo::ExecuteImpl(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	if (Method & STKWEBAPP_METHOD_GET) {
		return GetFilterInfo(UrlPath, ResultCode);
	} else
	if (Method & STKWEBAPP_METHOD_POST) {
		return PostFilterInfo(ReqObj, ResultCode);
	}
	return NULL;
}
