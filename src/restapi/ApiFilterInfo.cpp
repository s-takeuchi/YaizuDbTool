#include "MyMsgProc.h"
#include "ApiFilterInfo.h"
#include "dataaccess.h"
#include "..\Global.h"

StkObject* ApiFilterInfo::GetFilterInfo(wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode)
{
	StkObject* ResObj = new StkObject(L"");
	AddCodeAndMsg(ResObj, 0, L"", L"");
	StkObject* DatObj = new StkObject(L"Data");
	StkObject* FilInfObj = new StkObject(L"FilterInfo");
	DatObj->AppendChildElement(FilInfObj);

	BOOL FilterSw = DataAccess::GetInstance()->GetFilterSwitch();
	if (FilterSw) {
		FilInfObj->AppendChildElement(new StkObject(L"Function", L"enable"));
	} else {
		FilInfObj->AppendChildElement(new StkObject(L"Function", L"disable"));
	}
	for (int Loop = 1; Loop <= 5; Loop++) {
		wchar_t ColumnName[Global::COLUMNNAME_LENGTH];
		int FilterOpeType;
		wchar_t Value[Global::COLUMNVAL_LENGTH];

		DataAccess::GetInstance()->GetFilterCondition(Loop, ColumnName, &FilterOpeType, Value);
		StkObject* CriteriaObj = new StkObject(L"Criteria");
		CriteriaObj->AppendChildElement(new StkObject(L"index", Loop));
		CriteriaObj->AppendChildElement(new StkObject(L"columnname", ColumnName));
		CriteriaObj->AppendChildElement(new StkObject(L"opetype", FilterOpeType));
		CriteriaObj->AppendChildElement(new StkObject(L"value", Value));
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
		if (lstrcmp(CurObj->GetName(), L"Function") == 0) {
			wchar_t* StrFilterSw = CurObj->GetStringValue();
			if (StrFilterSw != NULL && lstrcmp(StrFilterSw, L"enable") == 0) {
				DataAccess::GetInstance()->SetFilterSwitch(TRUE);
			}
			if (StrFilterSw != NULL && lstrcmp(StrFilterSw, L"disable") == 0) {
				DataAccess::GetInstance()->SetFilterSwitch(FALSE);
			}
		}
		if (lstrcmp(CurObj->GetName(), L"Criteria") == 0) {
			StkObject* ChildObj = CurObj->GetFirstChildElement();
			int ValIndex = 0;
			wchar_t* PtrColumnname = NULL;
			int ValOpeType = -1;
			wchar_t* PtrValue = NULL;
			for (int Loop = 0; Loop < CurObj->GetChildElementCount(); Loop++) {
				if (lstrcmp(ChildObj->GetName(), L"index") == 0) {
					ValIndex = ChildObj->GetIntValue();
				} else if (lstrcmp(ChildObj->GetName(), L"columnname") == 0) {
					PtrColumnname = ChildObj->GetStringValue();
				} else if (lstrcmp(ChildObj->GetName(), L"opetype") == 0) {
					ValOpeType = ChildObj->GetIntValue();
				} else if (lstrcmp(ChildObj->GetName(), L"value") == 0) {
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

	StkObject* ResObj = new StkObject(L"");
	AddCodeAndMsg(ResObj, 0, L"", L"");

	return ResObj;
}

StkObject* ApiFilterInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3])
{
	if (Method & STKWEBAPP_METHOD_GET) {
		return GetFilterInfo(UrlPath, ResultCode);
	} else
	if (Method & STKWEBAPP_METHOD_POST) {
		return PostFilterInfo(ReqObj, ResultCode);
	}
	return NULL;
}
