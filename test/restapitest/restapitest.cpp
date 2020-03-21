#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

void TestGetSystem(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("GetSystem ... ");
	int ResultCode = 0;
	StkWebAppSendObj->SetAutholization("Bearer admin manager");
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/system/", NULL, &ResultCode);
	if (ResObj == NULL) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkObject* Dat = ResObj->GetFirstChildElement();
	bool ErrFlag = true;
	while (Dat) {
		if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0 && Dat->GetIntValue() != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
			Dat = Dat->GetFirstChildElement();
		}
		if (StkPlWcsCmp(Dat->GetName(), L"ProductName") == 0 && StkPlWcsCmp(Dat->GetStringValue(), L"CmdFreak") == 0) {
			ErrFlag = false;
		}
		if (StkPlWcsCmp(Dat->GetName(), L"ProductName") == 0 && StkPlWcsCmp(Dat->GetStringValue(), L"CmdFreak") != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		Dat = Dat->GetNext();
	}
	delete ResObj;
	if (ErrFlag) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\r\n");
}

void TestGetOdbcInfoDefault(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("GetOdbcInfo(query=default) ... ");
	int ResultCode = 0;
	StkWebAppSendObj->SetAutholization("Bearer admin manager");
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/odbcinfo/?query=default", NULL, &ResultCode);
	if (ResObj == NULL) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkObject* Dat = ResObj->GetFirstChildElement();
	bool ErrFlag = true;
	while (Dat) {
		if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0 && Dat->GetIntValue() != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
			if (StkPlWcsCmp(Dat->GetFirstChildElement()->GetName(), L"OdbcInfo") == 0) {
				wchar_t OdbcInfoStr[1024];
				Dat->ToJson(OdbcInfoStr, 1024);
				StkObject* PostgreObj = new StkObject(L"DbType", L"PostgreSQL");
				StkObject* MySqlObj = new StkObject(L"DbType", L"MySQL");
				StkObject* MariaDbObj = new StkObject(L"DbType", L"MariaDB");
				if (!Dat->Contains(PostgreObj) || !Dat->Contains(MySqlObj) || !Dat->Contains(MariaDbObj)) {
					StkPlPrintf("[NG]\r\n");
					StkPlExit(1);
				}
				delete PostgreObj;
				delete MySqlObj;
				delete MariaDbObj;
				ErrFlag = false;
			} else {
				StkPlPrintf("[NG]\r\n");
				StkPlExit(1);
			}
		}
		Dat = Dat->GetNext();
	}
	delete ResObj;
	if (ErrFlag) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\r\n");
}

void TestGetOdbcInfoConfigured(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("GetOdbcInfo(query=configured) ... ");
	int ResultCode = 0;
	StkWebAppSendObj->SetAutholization("Bearer admin manager");
	StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/odbcinfo/?query=configured", NULL, &ResultCode);
	if (ResObj == NULL) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkObject* Dat = ResObj->GetFirstChildElement();
	bool ErrFlag = true;
	while (Dat) {
		if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0 && Dat->GetIntValue() != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
			if (StkPlWcsCmp(Dat->GetFirstChildElement()->GetName(), L"OdbcInfo") == 0) {
				wchar_t OdbcInfoStr[1024];
				Dat->ToJson(OdbcInfoStr, 1024);
				StkObject* UnconnObj = new StkObject(L"Status", L"unconnectable");
				if (!Dat->Contains(UnconnObj)) {
					StkPlPrintf("[NG]\r\n");
					StkPlExit(1);
				}
				delete UnconnObj;
				ErrFlag = false;
			} else {
				StkPlPrintf("[NG]\r\n");
				StkPlExit(1);
			}
		}
		Dat = Dat->GetNext();
	}
	delete ResObj;
	if (ErrFlag) {
		StkPlPrintf("[NG]\r\n");
		StkPlExit(1);
	}
	StkPlPrintf("[OK]\r\n");
}

void TestGetUser(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("GetUser (no target, no ID/PW) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3124);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 401 || FoundObj == NULL || FoundObj->GetIntValue() != 3124) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* Dat = ResObj->GetFirstChildElement();
		int CodeInt = -1;
		wchar_t MsgEng[256] = L"";
		wchar_t MsgJpn[256] = L"";
		while (Dat) {
			if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0) {
				CodeInt = Dat->GetIntValue();
			}
			if (StkPlWcsCmp(Dat->GetName(), L"MsgEng") == 0) {
				StkPlWcsCpy(MsgEng, 256, Dat->GetStringValue());
			}
			if (StkPlWcsCmp(Dat->GetName(), L"MsgJpn") == 0) {
				StkPlWcsCpy(MsgJpn, 256, Dat->GetStringValue());
			}
			Dat = Dat->GetNext();
		}
		if (CodeInt != 3124 || StkPlWcsStr(MsgEng, L"Authentication error") == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("GetUser (no target, appropriate ID/PW) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResObj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* Dat = ResObj->GetFirstChildElement();
		int CodeInt = -1;
		wchar_t MsgEng[256] = L"";
		wchar_t MsgJpn[256] = L"";
		int TargetUserId = -1;
		while (Dat) {
			if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0) {
				CodeInt = Dat->GetIntValue();
			}
			if (StkPlWcsCmp(Dat->GetName(), L"MsgEng") == 0) {
				StkPlWcsCpy(MsgEng, 256, Dat->GetStringValue());
			}
			if (StkPlWcsCmp(Dat->GetName(), L"MsgJpn") == 0) {
				StkPlWcsCpy(MsgJpn, 256, Dat->GetStringValue());
			}
			if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
				StkObject* DatUser = Dat->GetFirstChildElement();
				if (StkPlWcsCmp(DatUser->GetName(), L"User") == 0) {
					StkObject* DatId = DatUser->GetFirstChildElement();
					while (DatId) {
						if (StkPlWcsCmp(DatId->GetName(), L"Id") == 0) {
							TargetUserId = DatId->GetIntValue();
						}
						DatId = DatId->GetNext();
					}
				}
			}
			Dat = Dat->GetNext();
		}
		if (CodeInt != 0 || MsgEng[0] != L'\0' || MsgJpn[0] != L'\0' || TargetUserId != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("GetUser (target=all, appropriate ID/PW, admin user) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/?target=all", NULL, &ResultCode);
		if (ResObj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* Dat = ResObj->GetFirstChildElement();
		int CodeInt = -1;
		wchar_t MsgEng[256] = L"";
		wchar_t MsgJpn[256] = L"";
		int UserCount = 0;
		while (Dat) {
			if (StkPlWcsCmp(Dat->GetName(), L"Code") == 0) {
				CodeInt = Dat->GetIntValue();
			}
			if (StkPlWcsCmp(Dat->GetName(), L"MsgEng") == 0) {
				StkPlWcsCpy(MsgEng, 256, Dat->GetStringValue());
			}
			if (StkPlWcsCmp(Dat->GetName(), L"MsgJpn") == 0) {
				StkPlWcsCpy(MsgJpn, 256, Dat->GetStringValue());
			}
			if (StkPlWcsCmp(Dat->GetName(), L"Data") == 0) {
				StkObject* DatUser = Dat->GetFirstChildElement();
				while (DatUser) {
					if (StkPlWcsCmp(DatUser->GetName(), L"User") == 0) {
						UserCount++;
					}
					DatUser = DatUser->GetNext();
				}
			}
			Dat = Dat->GetNext();
		}
		if (CodeInt != 0 || MsgEng[0] != L'\0' || MsgJpn[0] != L'\0' || UserCount != 2) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("GetUser (target=all, appropriate ID/PW, normal user) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer guest guest999");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/?target=all", NULL, &ResultCode);
		if (ResObj == NULL || ResultCode != 403) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
}

void TestPostUser(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("PostUser (add user, invalid ID/PW, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Name\" : \"testuser\", \"Role\" : 0, \"Password\" : \"testuser\"}", &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer error@a.a error");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3124);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 401 || FoundObj == NULL || FoundObj->GetIntValue() != 3124) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (add user, appropriate ID/PW, normal user, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Name\" : \"testuser\", \"Role\" : 0, \"Password\" : \"testuser\"}", &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer guest guest999");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 403) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (add user, appropriate ID/PW, admin user, no name specification, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Role\" : 0, \"Password\" : \"testuser\"}", &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3121);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 400 || FoundObj == NULL || FoundObj->GetIntValue() != 3121) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (add user, appropriate ID/PW, admin user, no role specification, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Name\" : \"testuser\", \"Password\" : \"testuser\"}", &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3121);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 400 || FoundObj == NULL || FoundObj->GetIntValue() != 3121) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (add user, appropriate ID/PW, admin user, no password specification, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Name\" : \"testuser\", \"Role\" : 0}", &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3133);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 400 || FoundObj == NULL || FoundObj->GetIntValue() != 3133) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	int TargetId = -1;
	{
		StkPlPrintf("PostUser (add user, appropriate ID/PW, admin user) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkObject* ReqObj = StkObject::CreateObjectFromJson(L"{\"Name\" : \"testuser\", \"Role\" : 0, \"Password\" : \"testuser\"}", &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 0);
		StkObject* Search2Obj = StkObject::CreateObjectFromJson(L"\"User\" : {\"Name\" : \"testuser\"}", &JsonRes);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		StkObject* Found2Obj = ResObj->Contains(Search2Obj);
		if (ResObj == NULL || ResultCode != 200 || FoundObj == NULL || Found2Obj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		Found2Obj = Found2Obj->GetFirstChildElement();
		while (Found2Obj) {
			if (StkPlWcsCmp(Found2Obj->GetName(), L"Id") == 0) {
				TargetId = Found2Obj->GetIntValue();
			}
			Found2Obj = Found2Obj->GetNext();
		}
		if (TargetId == -1) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete Search2Obj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, normal user, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : %d, \"Role\" : 0, \"Name\" :  \"testuser2\", \"Password\" : \"testuser2\"}", TargetId);
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer guest guest999");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 403) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, admin user, w/o password change) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : %d, \"Role\" : 0, \"Name\" :  \"testuser2\"}", TargetId);
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 200) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* SearchObj = new StkObject(L"Name", L"testuser2");
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (FoundObj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, admin user, invalid user ID, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : 100, \"Role\" : 0, \"Name\" :  \"testuser2\"}");
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 400) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* SearchObj = new StkObject(L"Code", 3128);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (FoundObj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, normal user, try to change admin's password, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : 0, \"Password\" : \"manager0\"}");
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer guest guest999");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 403) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* SearchObj = new StkObject(L"Code", 3125);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (FoundObj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, normal user, own password change) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : 1, \"Password\" : \"guest\"}");
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer guest guest999");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 200) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, admin user, try to change my own data, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : %d, \"Role\" : 0, \"Name\" :  \"testuser2\"}", TargetId);
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer testuser2 testuser");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 400) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* SearchObj = new StkObject(L"Code", 3126);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (FoundObj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, admin user, try to change user name into admin, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : %d, \"Role\" : 0, \"Name\" :  \"admin\"}", TargetId);
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 400) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkObject* SearchObj = new StkObject(L"Code", 3127);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (FoundObj == NULL) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("PostUser (modify user, appropriate ID/PW, admin user, Change only role) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		wchar_t ReqBuf[128] = L"";
		StkPlSwPrintf(ReqBuf, 128, L"{\"Id\" : %d, \"Role\" : 1}", TargetId);
		StkObject* ReqObj = StkObject::CreateObjectFromJson(ReqBuf, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL || ResultCode != 200) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
}

void TestDeleteUser(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("DeleteUser (invalid ID/PW, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkWebAppSendObj->SetAutholization("Bearer error@a.a error");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_DELETE, "/api/user/0/", NULL, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3124);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 401 || FoundObj == NULL || FoundObj->GetIntValue() != 3124) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("DeleteUser (access right error, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkWebAppSendObj->SetAutholization("Bearer guest guest");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_DELETE, "/api/user/0/", NULL, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3125);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 403 || FoundObj == NULL || FoundObj->GetIntValue() != 3125) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("DeleteUser (delete my own data, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_DELETE, "/api/user/0/", NULL, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3126);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 400 || FoundObj == NULL || FoundObj->GetIntValue() != 3126) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("DeleteUser (invalid user ID, abnormal) ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_DELETE, "/api/user/100/", NULL, &ResultCode);
		StkObject* SearchObj = new StkObject(L"Code", 3128);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (ResObj == NULL || ResultCode != 400 || FoundObj == NULL || FoundObj->GetIntValue() != 3128) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("DeleteUser ... ");
		int ResultCode = 0;
		int JsonRes = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_DELETE, "/api/user/2/", NULL, &ResultCode);
		if (ResObj == NULL || ResultCode != 200) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
}

void TestTooManyUsers(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("Too many users (add 64 users, abnormal) ... ");
	int ResultCode = 0;
	int JsonRes = 0;
	for (int Loop = 0; Loop < 64; Loop++) {
		wchar_t JsonReq[256] = L"";
		StkPlSwPrintf(JsonReq, 256, L"{\"Name\" : \"dummyuser%d\", \"Role\" : 1, \"Password\" : \"dummyuser%d\"}", Loop, Loop);
		StkObject* ReqObj = StkObject::CreateObjectFromJson(JsonReq, &JsonRes);
		StkWebAppSendObj->SetAutholization("Bearer admin manager");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/api/user/", ReqObj, &ResultCode);
		if (ResObj == NULL) {
			StkPlPrintf("[NG1]\r\n");
			StkPlExit(1);
		}
		StkObject* SearchObj = new StkObject(L"Code", 3134);
		StkObject* FoundObj = ResObj->Contains(SearchObj);
		if (Loop <= 61 && (FoundObj != NULL || ResultCode != 200)) {
			StkPlPrintf("[NG2]\r\n");
			StkPlExit(1);
		}
		if (Loop > 61 && (FoundObj == NULL || ResultCode != 400)) {
			StkPlPrintf("[NG3]\r\n");
			StkPlExit(1);
		}
		delete SearchObj;
		delete ReqObj;
		delete ResObj;

	}
	StkPlPrintf("[OK]\r\n");

}

void TestPostOperationStop(StkWebAppSend* StkWebAppSendObj)
{
	{
		StkPlPrintf("PostOperationStop (abnormal) ... ");
		int ResultCode = 0;
		StkObject* ReqObj = new StkObject(L"Operation", L"Stop");
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/service/", ReqObj, &ResultCode);
		StkObject* CurObj = ResObj->GetFirstChildElement();
		bool ProperErr = false;
		while (CurObj) {
			wchar_t* Name = CurObj->GetName();
			if (StkPlWcsCmp(Name, L"Code") == 0 && CurObj->GetIntValue() == 1004) {
				ProperErr = true;
			}
			CurObj = CurObj->GetNext();
		}
		delete ReqObj;
		delete ResObj;
		if (ProperErr == false) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\r\n");
	} 
	{
		StkPlPrintf("PostOperationStop (normal) ... ");
		int ResultCode = 0;
		StkObject* ReqObj = new StkObject(L"");
		ReqObj->AppendChildElement(new StkObject(L"Operation", L"Stop"));
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_POST, "/service/", ReqObj, &ResultCode);
		delete ReqObj;
		delete ResObj;
		StkPlPrintf("[OK]\r\n");
	}
}

int main(int Argc, char* Argv[])
{
	StkWebAppSend* StkWebAppSendObj = new StkWebAppSend(10, L"localhost", 10008);
	TestGetSystem(StkWebAppSendObj);
	TestGetOdbcInfoDefault(StkWebAppSendObj);
	TestGetOdbcInfoConfigured(StkWebAppSendObj);
	TestGetUser(StkWebAppSendObj);
	TestPostUser(StkWebAppSendObj);
	TestDeleteUser(StkWebAppSendObj);
	TestTooManyUsers(StkWebAppSendObj);
	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
