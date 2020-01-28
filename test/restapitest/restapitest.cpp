#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "../../../YaizuComLib/src/stkwebapp/StkWebAppSend.h"

void TestGetSystem(StkWebAppSend* StkWebAppSendObj)
{
	StkPlPrintf("GetSystem ... ");
	int ResultCode = 0;
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
		StkObject* ResObj = StkWebAppSendObj->SendRequestRecvResponse(StkWebAppSend::STKWEBAPP_METHOD_GET, "/api/user/", NULL, &ResultCode);
		if (ResObj == NULL) {
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
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("GetUser (no target, appropriate ID/PW) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin@a.a manager");
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
			if (StkPlWcsCmp(Dat->GetName(), L"User") == 0) {
				StkObject* DatId = Dat->GetFirstChildElement();
				while (DatId) {
					if (StkPlWcsCmp(DatId->GetName(), L"Id") == 0) {
						TargetUserId = DatId->GetIntValue();
					}
					DatId = DatId->GetNext();
				}
			}
			Dat = Dat->GetNext();
		}
		if (CodeInt != 0 || MsgEng[0] != L'\0' || MsgJpn[0] != L'\0' || TargetUserId != 0) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\r\n");
	}
	{
		StkPlPrintf("GetUser (target=all, appropriate ID/PW) ... ");
		int ResultCode = 0;
		StkWebAppSendObj->SetAutholization("Bearer admin@a.a manager");
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
			if (StkPlWcsCmp(Dat->GetName(), L"User") == 0) {
				UserCount++;
			}
			Dat = Dat->GetNext();
		}
		if (CodeInt != 0 || MsgEng[0] != L'\0' || MsgJpn[0] != L'\0' || UserCount != 2) {
			StkPlPrintf("[NG]\r\n");
			StkPlExit(1);
		}
		StkPlPrintf("[OK]\r\n");
	}
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
	TestPostOperationStop(StkWebAppSendObj);
	delete StkWebAppSendObj;
	return 0;
}
