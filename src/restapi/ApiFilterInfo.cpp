#include <windows.h>
#include <shlwapi.h>
#include <tchar.h>
#include "MyMsgProc.h"
#include "ApiFilterInfo.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiFilterInfo::GetFilterInfo(TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode)
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
	StkObject* CurObj = NULL;
	if (ReqObj != NULL) {
		CurObj = ReqObj->GetFirstChildElement();
	}
	while (CurObj) {
		if (lstrcmp(CurObj->GetName(), _T("Function")) == 0) {
			TCHAR* StrFilterSw = CurObj->GetStringValue();
			if (StrFilterSw != NULL && lstrcmp(StrFilterSw, _T("enable")) == 0) {
				DataAccess::GetInstance()->SetFilterSwitch(TRUE);
			}
			if (StrFilterSw != NULL && lstrcmp(StrFilterSw, _T("disable")) == 0) {
				DataAccess::GetInstance()->SetFilterSwitch(FALSE);
			}
		}
		if (lstrcmp(CurObj->GetName(), _T("Criteria")) == 0) {
			StkObject* ChildObj = CurObj->GetFirstChildElement();
			int ValIndex = 0;
			TCHAR* PtrColumnname = NULL;
			int ValOpeType = -1;
			TCHAR* PtrValue = NULL;
			for (int Loop = 0; Loop < CurObj->GetChildElementCount(); Loop++) {
				if (lstrcmp(ChildObj->GetName(), _T("index")) == 0) {
					ValIndex = ChildObj->GetIntValue();
				} else if (lstrcmp(ChildObj->GetName(), _T("columnname")) == 0) {
					PtrColumnname = ChildObj->GetStringValue();
				} else if (lstrcmp(ChildObj->GetName(), _T("opetype")) == 0) {
					ValOpeType = ChildObj->GetIntValue();
				} else if (lstrcmp(ChildObj->GetName(), _T("value")) == 0) {
					PtrValue = ChildObj->GetStringValue();
				}
				ChildObj = ChildObj->GetNext();
			}
			if (ValIndex >= 1 && ValIndex <= 5 && ValOpeType != -1 && PtrColumnname != NULL && PtrValue != NULL) {
				DataAccess::GetInstance()->SetFilterCondition(ValIndex, PtrColumnname, ValOpeType, PtrValue);
			}
		}
		CurObj = CurObj->GetNext();
	}
	
	DataAccess::GetInstance()->AddLogMsg(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_LOG_FILTERCHANGE), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_LOG_FILTERCHANGE));

	StkObject* ResObj = new StkObject(_T(""));
	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));

	return ResObj;
}

StkObject* ApiFilterInfo::ExecuteImpl(StkObject* ReqObj, int Method, TCHAR UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, TCHAR Locale[3])
{
	if (Method & STKWEBAPP_METHOD_GET) {
		return GetFilterInfo(UrlPath, ResultCode);
	} else
	if (Method & STKWEBAPP_METHOD_POST) {
		return PostFilterInfo(ReqObj, ResultCode);
	}
	return NULL;
}
