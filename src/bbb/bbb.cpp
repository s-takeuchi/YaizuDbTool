#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shlwapi.h>
#include "..\..\..\YaizuComLib\src\commonfunc\msgproc.h"
#include "MyMsgProc.h"
#include "BbbPage.h"
#include "confaccess.h"
#include "netaccessClient.h"
#include "..\Global.h"
#include "..\..\..\YaizuComLib\src\commonfunc\StkGeneric.h"


char AnalyzeAmpCode(char* OrgStr, char* EndPointer, int* PassByte)
{   
	int Ret;
	if (*OrgStr == '%' && (EndPointer - OrgStr) >= 6) {
		if (strncmp(OrgStr, "%0A%0D", 6) == 0 ||
			strncmp(OrgStr, "%0a%0a", 6) == 0 ||
			strncmp(OrgStr, "%0D%0A", 6) == 0 ||
			strncmp(OrgStr, "%0d%0a", 6) == 0) {
			*PassByte = 6;
			return '\n';
		}
	}
	if (*OrgStr == '%' && (EndPointer - OrgStr) >= 3) {
		*PassByte = 3;
		if (strncmp(OrgStr, "%0A", 3) == 0 ||
			strncmp(OrgStr, "%0a", 3) == 0 ||
			strncmp(OrgStr, "%0D", 3) == 0 ||
			strncmp(OrgStr, "%0d", 3) == 0) {
			return '\n';
		}
		char High = *(OrgStr + 1);
		char Low = *(OrgStr + 2);
		Ret = (High >= 'A' ? ((High & 0xDF) - 'A') + 10 : (High - '0'));
		Ret *= 16;
		Ret += (Low >= 'A' ? ((Low & 0xDF) - 'A') + 10 : (Low - '0'));
		return (char)Ret;
	}
	*PassByte = 1;
	return '%';
}

void UrlDecorde(char* OrgStr, char* TransStr, int SizeOfTransStr)
{
	int TransStrIndex = 0;
	int OrgStrLen = strlen(OrgStr);
	char* EndPoint = OrgStr + OrgStrLen;
	for (int Loop = 0; Loop < OrgStrLen; Loop++) {
		if (OrgStr[Loop] == '+') {
			TransStr[TransStrIndex] = ' ';
		} else if (OrgStr[Loop] == '%') {
			int PassByte;
			char Ret = AnalyzeAmpCode(&OrgStr[Loop], EndPoint, &PassByte);
			TransStr[TransStrIndex] = Ret;
			Loop += PassByte - 1;
		} else {
			TransStr[TransStrIndex] = OrgStr[Loop];
		}
		TransStrIndex++;
		if (TransStrIndex >= SizeOfTransStr -1) {
			break;
		}
	}
	TransStr[TransStrIndex] = '\0';
}

int Utf8ToWideCharSize(BYTE* Txt)
{
	return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)Txt, -1, NULL, NULL);
}

void Utf8ToWideChar(BYTE* Txt, TCHAR* WcTxt, int WcSize)
{
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)Txt, -1, WcTxt, WcSize);
}

// This method acquire HTTP message for GET/PUT.
// GetHttpMessage [out]: Acquired GET message
// PostHttpMessage [out]: Acquired PUT message
// Return: Actual size (in byte) of acquired HTTP message. If an error occurred in the acquisition, -1 is returned.
int AcquireHttpMessage(BYTE GetHttpMessage[Global::MAX_HTTP_REQUEST_SIZE], BYTE PostHttpMessage[Global::MAX_HTTP_REQUEST_SIZE])
{
	char* Method;
	size_t MethodSize;

	GetHttpMessage[0] = 0;
	PostHttpMessage[0] = 0;
	if (_dupenv_s(&Method, &MethodSize, "REQUEST_METHOD") == 0) {
		if (Method == 0 || MethodSize == 0) {
			return -1;
		}
		if (strcmp(Method, "GET") == 0) {
			free(Method);
			char* Query;
			size_t QuerySize;
			if (_dupenv_s(&Query, &QuerySize, "QUERY_STRING") == 0) {
				if (Query == 0 || QuerySize == 0) {
					GetHttpMessage[0] = 0;
				} else {
					strcpy_s((char*)GetHttpMessage, QuerySize, Query);
					free(Query);
				}
				return QuerySize;
			}
			return -1;
		}
		if (strcmp(Method, "POST") == 0) {
			free(Method);
			char* Length;
			size_t LengthSize;
			if (_dupenv_s(&Length, &LengthSize, "CONTENT_LENGTH") == 0) {
				if (Length == 0 || LengthSize == 0) {
					return -1;
				}
				int LengthInt = atoi(Length);
				if (LengthInt > Global::MAX_HTTP_REQUEST_SIZE) {
					LengthInt = Global::MAX_HTTP_REQUEST_SIZE - 1;
				}
				fgets((char*)PostHttpMessage, LengthInt + 1, stdin);
				PostHttpMessage[LengthInt] = '\0';
				free(Length);
				return LengthInt;
			}
			return -1;
		}
	}
	return -1;
}

int Dispatcher(BYTE GetHttpMessage[Global::MAX_HTTP_REQUEST_SIZE], BYTE PostHttpMessage[Global::MAX_HTTP_REQUEST_SIZE])
{
	TCHAR Hostname[256];
	int PortNum;
	ConfAccess::GetInstance()->GetHostnameAndPortFromFile(Hostname, &PortNum);

	NetAccessClient* NetAcCli = NetAccessClient::GetInstance();

	// Only index.cgi is specified.
	if (GetHttpMessage[0] == 0 && PostHttpMessage[0] == 0) {
		// Initial page
		BbbPage::GetInstance()->MainPage(BbbPage::MAINPAGE_INIT, NULL, NULL, NULL, NULL);
		return 0;
	}

	if (GetHttpMessage[0] != 0) {
		// Unexpected case
		BbbPage::GetInstance()->MainPage(BbbPage::MAINPAGE_UNEXPECTED_ERROR, NULL, NULL, NULL, NULL);
		return -1;
	}

	if (PostHttpMessage[0] != 0) {
		BYTE PrevPostHttpMessage[Global::MAX_HTTP_REQUEST_SIZE] = "\0";

		// Check the TestConn result. If invalid, an error occurs.
		if ((strstr((char*)PostHttpMessage, "operation=test") != 0)) {
			if (NetAcCli->TestConn(Hostname, PortNum) == -1) {
				BbbPage::GetInstance()->OutputCommandResult(_T("test"), BbbPage::RESULTCODE_SERVICEDOWN_ERROR, NULL);
				return -1;
			}
			if (NetAcCli->TestConn(Hostname, PortNum) == -2) {
				BbbPage::GetInstance()->OutputCommandResult(_T("test"), BbbPage::RESULTCODE_DBMSCONNSTR_ERROR, NULL);
				return -1;
			}
			if (NetAcCli->TestConn(Hostname, PortNum) == -3) {
				BbbPage::GetInstance()->OutputCommandResult(_T("test"), BbbPage::RESULTCODE_EMPTYCONNSTR_ERROR, NULL);
				return -1;
			}
			BbbPage::GetInstance()->OutputCommandResult(_T("test"), BbbPage::RESULTCODE_SUCCESS, NULL);
			return 0;
		}

		// Get build and service start time
		if ((strstr((char*)PostHttpMessage, "operation=config-time") != 0)) {
			char BuildDate[32];
			char ServiceStartTime[32];
			NetAcCli->GetSvInf(Hostname, PortNum, BuildDate, ServiceStartTime);
			TCHAR Data[Global::MAX_PARAM_LENGTH];
			_snwprintf_s(Data, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("{\"buildtime\" : \"%S\", \"starttime\" : \"%S\"}"), BuildDate, ServiceStartTime);
			BbbPage::GetInstance()->OutputCommandResult(_T("config-time"), BbbPage::RESULTCODE_SUCCESS, Data);
			return 0;
		}

		// Get default connection string
		if ((strstr((char*)PostHttpMessage, "operation=get-defconnstr") != 0)) {
			int PoHttpMsgLength = strlen((char*)PostHttpMessage) + 1;
			BYTE* PoHttpMsg = new BYTE[PoHttpMsgLength];
			strcpy_s((char*)PoHttpMsg, PoHttpMsgLength, (char*)PostHttpMessage);
			char* TmpDbmsType = (char*)(strstr((char*)PoHttpMsg, "dbmstype=") + 9);
			for (char* PoHttpMsgAddr = (char*)PoHttpMsg; PoHttpMsgAddr < (char*)(PoHttpMsgLength + PoHttpMsg); PoHttpMsgAddr++) {
				if (*PoHttpMsgAddr == '&') {
					*PoHttpMsgAddr = '\0';
				}
			}
			char DbmsType[20] = "";
			strcpy_s(DbmsType, 20, TmpDbmsType);
			TCHAR ConnStr[Global::MAX_PARAM_LENGTH];
			NetAcCli->GetDfCnS(Hostname, PortNum, DbmsType, ConnStr);
			TCHAR Data[Global::MAX_PARAM_LENGTH];
			_snwprintf_s(Data, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("{\"defconnstr\" : \"%s\"}"), ConnStr);
			BbbPage::GetInstance()->OutputCommandResult(_T("get-defconnstr"), BbbPage::RESULTCODE_SUCCESS, Data);
			delete PoHttpMsg;
			return 0;
		}

		// Set connection string
		if ((strstr((char*)PostHttpMessage, "operation=set-connstr") != 0)) {
			int PoHttpMsgLength = strlen((char*)PostHttpMessage) + 1;
			BYTE* PoHttpMsg = new BYTE[PoHttpMsgLength];
			strcpy_s((char*)PoHttpMsg, PoHttpMsgLength, (char*)PostHttpMessage);
			char* TmpConnStr  = (char*)(strstr((char*)PoHttpMsg, "connstr=") + 8);
			char* TmpDbmsType = (char*)(strstr((char*)PoHttpMsg, "dbmstype=") + 9);
			for (char* PoHttpMsgAddr = (char*)PoHttpMsg; PoHttpMsgAddr < (char*)(PoHttpMsgLength + PoHttpMsg); PoHttpMsgAddr++) {
				if (*PoHttpMsgAddr == '&') {
					*PoHttpMsgAddr = '\0';
					break; // If connstr contains '&', do not conver it to '\0'.
				}
			}
			int DbmsType = atoi(TmpDbmsType);
			int LenOfConnStrWc = Utf8ToWideCharSize((BYTE*)TmpConnStr);
			if (LenOfConnStrWc > 256) {
				BbbPage::GetInstance()->OutputCommandResult(_T("set-connstr"), BbbPage::RESULTCODE_TOOLONGCONNSTR_ERROR, NULL);
				delete PoHttpMsg;
				return -1;
			}
			TCHAR* ConnStrWc = new TCHAR[LenOfConnStrWc + 1];
			Utf8ToWideChar((BYTE*)TmpConnStr, ConnStrWc, LenOfConnStrWc + 1);
			TCHAR ConnStr[300];
			_snwprintf_s(ConnStr, 300, _TRUNCATE, _T("%d %s"), DbmsType, ConnStrWc);
			NetAcCli->SetConSt(Hostname, PortNum, DbmsType, ConnStr);
			BbbPage::GetInstance()->OutputCommandResult(_T("set-connstr"), BbbPage::RESULTCODE_SUCCESS, NULL);
			delete ConnStrWc;
			delete PoHttpMsg;
			return 0;
		}

		// Get connection string
		if ((strstr((char*)PostHttpMessage, "operation=get-connstr") != 0)) {
			TCHAR Data[Global::MAX_PARAM_LENGTH];
			NetAcCli->GetConSt(Hostname, PortNum, Data);
			BbbPage::GetInstance()->OutputCommandResult(_T("get-connstr"), BbbPage::RESULTCODE_SUCCESS, Data);
			return 0;
		}

		// Check log output
		if ((strstr((char*)PostHttpMessage, "operation=config-checklog") != 0)) {
			TCHAR LogOutput[Global::MAX_PARAM_LENGTH / 2];
			NetAcCli->LogsHtml(Hostname, PortNum, LogOutput);
			TCHAR Data[Global::MAX_PARAM_LENGTH];
			_snwprintf_s(Data, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("{\"log\" : \"%s\"}"), LogOutput);
			BbbPage::GetInstance()->OutputCommandResult(_T("config-checklog"), BbbPage::RESULTCODE_SUCCESS, Data);
			return 0;
		}

		// Get records
		if ((strstr((char*)PostHttpMessage, "operation=get-records") != 0)) {
			int PoHttpMsgLength = strlen((char*)PostHttpMessage) + 1;
			BYTE* PoHttpMsg = new BYTE[PoHttpMsgLength];
			strcpy_s((char*)PoHttpMsg, PoHttpMsgLength, (char*)PostHttpMessage);
			char* TmpTableName  = (char*)(strstr((char*)PoHttpMsg, "tablename=") + 10);
			TCHAR TableName[Global::TABLENAME_LENGTH];
			Utf8ToWideChar((BYTE*)TmpTableName, TableName, Global::TABLENAME_LENGTH);

			int NumOfRecs = NetAcCli->GetNumOfRecs(Hostname, PortNum, TableName);
			// In case of specified table does not exist.
			if (NumOfRecs < 0) {
				BbbPage::GetInstance()->OutputCommandResult(_T("get-records"), BbbPage::RESULTCODE_SUCCESS, _T("[]"));
				delete PoHttpMsg;
				return 0;
			}

			int Length = NumOfRecs *  Global::MAXNUM_COLUMNS * (Global::COLUMNVAL_LENGTH + 10);
			TCHAR* Data = new TCHAR[Length + 1000];
			if (Length == 0) {
				lstrcpy(Data, _T("[]"));
			} else {
				NetAcCli->GetRecrd(Hostname, PortNum, Data, Length, TableName);
			}
			BbbPage::GetInstance()->OutputCommandResult(_T("get-records"), BbbPage::RESULTCODE_SUCCESS, Data);
			delete Data;
			delete PoHttpMsg;
			return 0;
		}

		// Get columns
		if ((strstr((char*)PostHttpMessage, "operation=get-columns") != 0)) {
			int PoHttpMsgLength = strlen((char*)PostHttpMessage) + 1;
			BYTE* PoHttpMsg = new BYTE[PoHttpMsgLength];
			strcpy_s((char*)PoHttpMsg, PoHttpMsgLength, (char*)PostHttpMessage);
			BYTE* TmpTableName  = (BYTE*)(strstr((char*)PoHttpMsg, "tablename=") + 10);

			int LenOfTableNameWc = Utf8ToWideCharSize(TmpTableName);
			TCHAR* TableNameWc = new TCHAR[LenOfTableNameWc + 1];
			Utf8ToWideChar(TmpTableName, TableNameWc, LenOfTableNameWc + 1);
			TCHAR DecodedTableName[Global::TABLENAME_LENGTH];
			StkGeneric::GetInstance()->HtmlDecode(TableNameWc, DecodedTableName, Global::TABLENAME_LENGTH);

			TCHAR Data[Global::MAX_PARAM_LENGTH];
			NetAcCli->GetColms(Hostname, PortNum, Data, DecodedTableName);
			BbbPage::GetInstance()->OutputCommandResult(_T("get-columns"), BbbPage::RESULTCODE_SUCCESS, Data);
			delete TableNameWc;
			delete PoHttpMsg;
			return 0;
		}

		// Get table names
		if ((strstr((char*)PostHttpMessage, "operation=get-tablenames") != 0)) {
			TCHAR Data[Global::MAX_PARAM_LENGTH];
			NetAcCli->GetTblns(Hostname, PortNum, Data);
			BbbPage::GetInstance()->OutputCommandResult(_T("get-tablenames"), BbbPage::RESULTCODE_SUCCESS, Data);
			return 0;
		}

		// Set filter switch on
		if ((strstr((char*)PostHttpMessage, "operation=filtersw_on") != 0)) {
			NetAcCli->FilterSw(Hostname, PortNum, TRUE);
			BbbPage::GetInstance()->OutputCommandResult(_T("filtersw_on"), BbbPage::RESULTCODE_SUCCESS, NULL);
			return 0;
		}

		// Set filter switch off
		if ((strstr((char*)PostHttpMessage, "operation=filtersw_off") != 0)) {
			NetAcCli->FilterSw(Hostname, PortNum, FALSE);
			BbbPage::GetInstance()->OutputCommandResult(_T("filtersw_on"), BbbPage::RESULTCODE_SUCCESS, NULL);
			return 0;
		}

		// Get filter switch
		if ((strstr((char*)PostHttpMessage, "operation=getfiltersw") != 0)) {
			BOOL RetGetFilSw = NetAcCli->GetFilSw(Hostname, PortNum);
			TCHAR Data[Global::MAX_PARAM_LENGTH];
			_snwprintf_s(Data, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("{\"switchon\" : %s}"), (RetGetFilSw == TRUE)? _T("true") : _T("false"));
			BbbPage::GetInstance()->OutputCommandResult(_T("getfiltersw"), BbbPage::RESULTCODE_SUCCESS, Data);
			return 0;
		}

		// Set filter condition
		if ((strstr((char*)PostHttpMessage, "operation=setfiltercd") != 0)) {
			int PoHttpMsgLength = strlen((char*)PostHttpMessage) + 1;
			BYTE* PoHttpMsg = new BYTE[PoHttpMsgLength];
			strcpy_s((char*)PoHttpMsg, PoHttpMsgLength, (char*)PostHttpMessage);
			BYTE* TmpParamIdx  = (BYTE*)(strstr((char*)PoHttpMsg, "paramidx="));
			BYTE* TmpParamCol  = (BYTE*)(strstr((char*)PoHttpMsg, "paramcol="));
			BYTE* TmpParamOpe  = (BYTE*)(strstr((char*)PoHttpMsg, "paramope="));
			BYTE* TmpParamVal  = (BYTE*)(strstr((char*)PoHttpMsg, "paramval="));
			*(TmpParamIdx - 1) = '\0';
			*(TmpParamCol - 1) = '\0';
			*(TmpParamOpe - 1) = '\0';
			*(TmpParamVal - 1) = '\0';
			TmpParamIdx += 9;
			TmpParamCol += 9;
			TmpParamOpe += 9;
			TmpParamVal += 9;
			int ParamIdx = atoi((char*)TmpParamIdx);
			int ParamOpe = atoi((char*)TmpParamOpe);
			int LenOfParamCol = Utf8ToWideCharSize(TmpParamCol);
			int LenOfParamVal = Utf8ToWideCharSize(TmpParamVal);
			if (LenOfParamVal >= Global::COLUMNVAL_LENGTH) {
				BbbPage::GetInstance()->OutputCommandResult(_T("none"), BbbPage::RESULTCODE_TOOLONGFILTER_ERROR, NULL);
				return -1;
			}
			TCHAR* ParamCol = new TCHAR[LenOfParamCol + 1];
			TCHAR* ParamVal = new TCHAR[LenOfParamVal + 1];
			Utf8ToWideChar(TmpParamCol, ParamCol, LenOfParamCol + 1);
			Utf8ToWideChar(TmpParamVal, ParamVal, LenOfParamVal + 1);
			NetAcCli->SetFilCd(Hostname, PortNum, ParamIdx, ParamCol, ParamOpe, ParamVal);
			BbbPage::GetInstance()->OutputCommandResult(_T("setfiltercd"), BbbPage::RESULTCODE_SUCCESS, NULL);
			delete ParamCol;
			delete ParamVal;
			delete PoHttpMsg;
			return 0;
		}

		// Get filter condition
		if ((strstr((char*)PostHttpMessage, "operation=getfiltercd") != 0)) {
			TCHAR Data[Global::MAX_PARAM_LENGTH];
			NetAcCli->GetFilCd(Hostname, PortNum, Data);
			BbbPage::GetInstance()->OutputCommandResult(_T("getfiltercd"), BbbPage::RESULTCODE_SUCCESS, Data);
			return 0;
		}

		BbbPage::GetInstance()->OutputCommandResult(_T("none"), BbbPage::RESULTCODE_UNEXPECTED_ERROR, NULL);
		return -1;
	}

	return -1;
}

int main(int argc, char* argv[])
{
	// Time for attaching this process to debugger
	#ifdef _DEBUG
		Sleep(10 * 1000);
	#endif

	MessageProc::SetLocaleMode(MessageProc::LOCALE_MODE_WEB);
	MyMsgProc::AddMsg();

	BYTE GetHttpMessage[Global::MAX_HTTP_REQUEST_SIZE];
	BYTE PostHttpMessage[Global::MAX_HTTP_REQUEST_SIZE];
	if (AcquireHttpMessage(GetHttpMessage, PostHttpMessage) == -1) {
		BbbPage::GetInstance()->MainPage(BbbPage::MAINPAGE_UNEXPECTED_ERROR, NULL, NULL, NULL, NULL);
	} else {
		Dispatcher(GetHttpMessage, PostHttpMessage);
	}

	return 0;
}
