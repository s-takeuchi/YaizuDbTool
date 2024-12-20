﻿#include "../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.h"
#include "MyMsgProc.h"
#include "ApiFilterInfo.h"
#include "dataaccess.h"
#include "Global.h"

StkObject* ApiFilterInfo::GetFilterInfo(wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode)
{
	StkObject* ResObj = new StkObject(L"");
	AddCodeAndMsg(ResObj, 0, L"", L"");
	StkObject* DatObj = new StkObject(L"Data");
	StkObject* FilInfObj = new StkObject(L"FilterInfo");
	DatObj->AppendChildElement(FilInfObj);

	bool FilterSw = DataAccess::GetInstance()->GetFilterSwitch();
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

StkObject* ApiFilterInfo::PostFilterInfo(StkObject* ReqObj, int* ResultCode, int UserId)
{
	StkObject* CurObj = NULL;
	if (ReqObj != NULL) {
		CurObj = ReqObj->GetFirstChildElement();
	}
	bool UpdatedFlag = false;
	while (CurObj) {
		if (StkPlWcsCmp(CurObj->GetName(), L"Function") == 0) {
			wchar_t* StrFilterSw = CurObj->GetStringValue();
			if (StrFilterSw != NULL && StkPlWcsCmp(StrFilterSw, L"enable") == 0) {
				DataAccess::GetInstance()->SetFilterSwitch(true);
			}
			if (StrFilterSw != NULL && StkPlWcsCmp(StrFilterSw, L"disable") == 0) {
				DataAccess::GetInstance()->SetFilterSwitch(false);
			}
		}
		if (StkPlWcsCmp(CurObj->GetName(), L"Criteria") == 0) {
			StkObject* ChildObj = CurObj->GetFirstChildElement();
			int ValIndex = 0;
			wchar_t* PtrColumnname = NULL;
			int ValOpeType = -1;
			wchar_t* PtrValue = NULL;
			for (int Loop = 0; Loop < CurObj->GetChildElementCount(); Loop++) {
				if (StkPlWcsCmp(ChildObj->GetName(), L"index") == 0) {
					ValIndex = ChildObj->GetIntValue();
				} else if (StkPlWcsCmp(ChildObj->GetName(), L"columnname") == 0) {
					PtrColumnname = ChildObj->GetStringValue();
				} else if (StkPlWcsCmp(ChildObj->GetName(), L"opetype") == 0) {
					ValOpeType = ChildObj->GetIntValue();
				} else if (StkPlWcsCmp(ChildObj->GetName(), L"value") == 0) {
					PtrValue = ChildObj->GetStringValue();
				}
				ChildObj = ChildObj->GetNext();
			}
			if (ValIndex >= 1 && ValIndex <= 5 && ValOpeType != -1 && PtrColumnname != NULL && PtrValue != NULL) {
				wchar_t TmpColumnname[Global::COLUMNNAME_LENGTH] = L"";
				int TmpOpeType = -1;
				wchar_t TmpValue[Global::COLUMNVAL_LENGTH] = L"";
				DataAccess::GetInstance()->GetFilterCondition(ValIndex, TmpColumnname, &TmpOpeType, TmpValue);
				if (StkPlWcsCmp(TmpColumnname, PtrColumnname) != 0 || TmpOpeType != ValOpeType || StkPlWcsCmp(TmpValue, PtrValue) != 0) {
					UpdatedFlag = true;
				}
				DataAccess::GetInstance()->SetFilterCondition(ValIndex, PtrColumnname, ValOpeType, PtrValue);
			}
		}
		CurObj = CurObj->GetNext();
	}

	if (UpdatedFlag) {
		Global::EventLogging(MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_LOG_FILTERCHANGE), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_LOG_FILTERCHANGE), UserId);
	}

	StkObject* ResObj = new StkObject(L"");
	AddCodeAndMsg(ResObj, 0, L"", L"");

	return ResObj;
}

StkObject* ApiFilterInfo::ExecuteImpl(StkObject* ReqObj, int Method, wchar_t UrlPath[StkWebAppExec::URL_PATH_LENGTH], int* ResultCode, wchar_t Locale[3], wchar_t* Token)
{
	wchar_t YourName[Global::MAXLEN_OF_USERNAME] = L"";
	int YourId = -1;
	if (!CheckCredentials(Token, YourName, &YourId)) {
		StkObject* ResObj = new StkObject(L"");
		AddCodeAndMsg(ResObj, MyMsgProc::CMDFRK_AUTH_ERROR, MyMsgProc::GetMsgEng(MyMsgProc::CMDFRK_AUTH_ERROR), MyMsgProc::GetMsgJpn(MyMsgProc::CMDFRK_AUTH_ERROR));
		*ResultCode = 401;
		return ResObj;
	}

	if (Method & STKWEBAPP_METHOD_GET) {
		return GetFilterInfo(UrlPath, ResultCode);
	} else
	if (Method & STKWEBAPP_METHOD_POST) {
		return PostFilterInfo(ReqObj, ResultCode, YourId);
	}
	return NULL;
}
