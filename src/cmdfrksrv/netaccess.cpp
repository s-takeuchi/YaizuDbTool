#include "..\..\com\stksocket\stksocket.h"
#include <windows.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "netaccess.h"
#include "dataaccess.h"
#include "OdbcManager.h"
#include "DbAccessor.h"
#include "..\Global.h"
#include "..\..\com\commonfunc\StkGeneric.h"
#include "..\..\com\msgproc\msgproc.h"

NetAccess* NetAccess::ThisInstance;

NetAccess::NetAccess()
{
	StkGeneric::GetInstance()->GetLocalTimeStr(ServiceStartTime);
}

NetAccess::~NetAccess()
{
}

// Get this instance
NetAccess* NetAccess::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new NetAccess();
		Init = 0;
	}
	return ThisInstance;
}

// This function processes the request from client.
// Id [in] : StkSocketId
// Status [in] : Status of initialize process (0: Normal case, -1:Data file not found)
// Return : Always zero returned.
int NetAccess::Dispatcher(int Id, int Status)
{
	BYTE Command[20];

	if (StkSocket_Accept(Id) == 0) {
		int Ret = StkSocket_Receive(Id, Id, Command, 20, 10000020, (BYTE*)NULL, 0, FALSE);
		if (Ret > 0) {
			Command[19] = NULL;
			int DataSize = atoi((char*)Command + 10);

			// Logging for Command
			/*{
				TCHAR LogBuf[32];
				_snwprintf_s(LogBuf, 32, _TRUNCATE, _T("%S START"), Command);
				DataAccess::GetInstance()->AddLogMsg((TCHAR*)LogBuf);
			}*/

			///// TestConn /////
			if (memcmp(Command, (char*)"[TestConn]", 10) == 0) {
				TCHAR ConnStr[256];
				int Init;
				int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
				if (Init == 1) {
					StkSocket_Send(Id, Id, (BYTE*)"EM", 2);
				} else {
					DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
					int Ret = Da->Test(ConnStr);
					OdbcManager::GetInstance()->DeleteAccessorObject(Da);
					if (Ret == SQL_SUCCESS) {
						StkSocket_Send(Id, Id, (BYTE*)"OK", 2);
					} else {
						StkSocket_Send(Id, Id, (BYTE*)"NG", 2);
					}
				}
			}

			///// GetDfCnS /////
			if (memcmp(Command, (char*)"[GetDfCnS]", 10) == 0) {
				BYTE* DbmsType = new BYTE[DataSize + 1];
				int Ret = StkSocket_Receive(Id, Id, DbmsType, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				DbmsType[DataSize] = '\0';
				DbAccessor* Da = NULL;
				if (strcmp((char*)DbmsType, "postgresql") == 0) {
					Da = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::POSTGRESQL_ACCESSOR);
				}
				if (strcmp((char*)DbmsType, "mariadb") == 0) {
					Da = OdbcManager::GetInstance()->CreateAccessorObject(OdbcManager::MARIADB_ACCESSOR);
				}
				SQLTCHAR DefConnStr[Global::MAX_PARAM_LENGTH];
				if (Da == NULL) {
					lstrcpy(DefConnStr, _T("\0"));
				} else {
					Da->GetDefaultConnStr(DefConnStr);
				}
				StkSocket_Send(Id, Id, (BYTE*)DefConnStr, (lstrlen(DefConnStr) + 1) * sizeof(TCHAR));
				OdbcManager::GetInstance()->DeleteAccessorObject(Da);
				delete DbmsType;
			}

			///// SetConSt /////
			if (memcmp(Command, (char*)"[SetConSt]", 10) == 0) {
				BYTE* OdbcConnStr = new BYTE[DataSize + 1];
				int Ret = StkSocket_Receive(Id, Id, OdbcConnStr, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				OdbcConnStr[1] = 0;
				int DbmsType = _wtoi((TCHAR*)OdbcConnStr);
				DataAccess::GetInstance()->SetOdbcConnStr(DbmsType, (TCHAR*)OdbcConnStr + 2);
				StkSocket_Send(Id, Id, (BYTE*)"OK", 2);
				delete OdbcConnStr;

				TCHAR Buf[1024];
				TCHAR DbmsTypeName[64];
				if (DbmsType == 0) {
					lstrcpy(DbmsTypeName, _T("MySQL/MariaDB"));
				} else {
					lstrcpy(DbmsTypeName, _T("PostgreSQL"));
				}
				_snwprintf_s(Buf, 1024, _TRUNCATE, _T("%s [%s]"), MessageProc::GetMsg(MessageProc::CMDFRK_LOG_DBMSCHANGE), DbmsTypeName);
				DataAccess* DatAc = DataAccess::GetInstance();
				DatAc->AddLogMsg(Buf);
			}

			///// GetConSt /////
			if (memcmp(Command, (char*)"[GetConSt]", 10) == 0) {
				TCHAR ConnStr[256];
				TCHAR ConvConnStr[256 * 5];
				TCHAR ConvConnStrTmp[256 * 5];
				int Init;
				int Ret = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
				StkGeneric::GetInstance()->HtmlEncode(ConnStr, ConvConnStrTmp, 256 * 5);
				StkGeneric::GetInstance()->JsonEncode(ConvConnStrTmp, ConvConnStr, 256 * 5);
				TCHAR Data[Global::MAX_PARAM_LENGTH];
				_snwprintf_s(Data, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("{\"DbmsType\" : %d, \"ConnStr\" : \"%s\"}"), Ret, ConvConnStr);
				StkSocket_Send(Id, Id, (BYTE*)Data, (lstrlen(Data) + 1) * sizeof(TCHAR));
			}

			///// GetSvInf /////
			if (memcmp(Command, (char*)"[GetSvInf]", 10) == 0) {
				char BuildDate[32];
				sprintf_s(BuildDate, 32, "%s %s", __DATE__, __TIME__);
				StkSocket_Send(Id, Id, (BYTE*)BuildDate, 32);
				StkSocket_Send(Id, Id, (BYTE*)ServiceStartTime, 32);
			}

			///// GetNumOR /////
			if (memcmp(Command, (char*)"[GetNumOR]", 10) == 0) {
				BYTE* TableName = new BYTE[DataSize + 2];
				int Ret = StkSocket_Receive(Id, Id, TableName, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				TableName[DataSize] = '\0';
				TableName[DataSize + 1] = '\0';

				SQLTCHAR StateMsg[10];
				SQLTCHAR Msg[1024];
				TCHAR ConnStr[256];
				int Init;

				int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
				DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
				INT32 TmpNumOfRecs = (INT32)Da->GetNumOfRecords((SQLTCHAR*)TableName, StateMsg, Msg, 1024);
				OdbcManager::GetInstance()->DeleteAccessorObject(Da);
				StkSocket_Send(Id, Id, (BYTE*)&TmpNumOfRecs, 4);
			}

			///// GetRecrd /////
			if (memcmp(Command, (char*)"[GetRecrd]", 10) == 0) {
				BYTE* TableName = new BYTE[DataSize + 2];
				int Ret = StkSocket_Receive(Id, Id, TableName, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				TableName[DataSize] = '\0';
				TableName[DataSize + 1] = '\0';

				SQLTCHAR ColumnName[Global::MAXNUM_COLUMNS][Global::COLUMNNAME_LENGTH];
				SQLTCHAR ColumnType[Global::MAXNUM_COLUMNS][Global::COLUMNTYPE_LENGTH];
				SQLTCHAR IsNull[Global::MAXNUM_COLUMNS][10];
				SQLTCHAR StateMsg[10];
				SQLTCHAR Msg[1024];

				TCHAR ConnStr[256];
				int Init;
				int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
				DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
				int TmpNumOfRecs = Da->GetNumOfRecords((SQLTCHAR*)TableName, StateMsg, Msg, 1024);
				SQLTCHAR* Record = new SQLTCHAR[TmpNumOfRecs * Global::MAXNUM_COLUMNS * Global::COLUMNVAL_LENGTH];
				int NumOfCols = Da->GetColumnInfoByTableName((SQLTCHAR*)TableName, ColumnName, ColumnType, IsNull, StateMsg, Msg, 1024);
				int NumOfRecs = Da->GetRecordsByTableName((SQLTCHAR*)TableName, NumOfCols, Record, TmpNumOfRecs, StateMsg, Msg, 1024);
				OdbcManager::GetInstance()->DeleteAccessorObject(Da);
				delete TableName;

				StkSocket_Send(Id, Id, (BYTE*)_T("["), 2);
				for (int LoopRec = 0; LoopRec < NumOfRecs; LoopRec++) {
					TCHAR Data[Global::MAX_PARAM_LENGTH] = _T("");
					lstrcat(Data, _T("{ "));
					for (int LoopCol = 0; LoopCol < NumOfCols; LoopCol++) {
						TCHAR TmpColumnVal[Global::COLUMNVAL_LENGTH * 5];
						TCHAR TmpColumnVal2[Global::COLUMNVAL_LENGTH * 5];
						StkGeneric::GetInstance()->HtmlEncode(&Record[LoopRec * Global::MAXNUM_COLUMNS * Global::COLUMNVAL_LENGTH + LoopCol * Global::COLUMNVAL_LENGTH], TmpColumnVal2, Global::COLUMNVAL_LENGTH * 5);
						StkGeneric::GetInstance()->JsonEncode(TmpColumnVal2, TmpColumnVal, Global::COLUMNVAL_LENGTH * 5);
						TCHAR TmpData[Global::MAX_PARAM_LENGTH];
						_snwprintf_s(TmpData, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("\"%d\": \"%s\""), LoopCol, TmpColumnVal);
						lstrcat(Data, TmpData);
						if (LoopCol + 1 < NumOfCols) {
							lstrcat(Data, _T(","));
						}
					}
					lstrcat(Data, _T("}"));
					if (LoopRec + 1 < NumOfRecs) {
						lstrcat(Data, _T(","));
					}
					StkSocket_Send(Id, Id, (BYTE*)Data, lstrlen(Data) * sizeof(TCHAR));
				}
				StkSocket_Send(Id, Id, (BYTE*)_T("]\0"), 4);
				delete Record;
			}

			///// GetColms /////
			if (memcmp(Command, (char*)"[GetColms]", 10) == 0) {
				BYTE* TableName = new BYTE[DataSize + 2];
				int Ret = StkSocket_Receive(Id, Id, TableName, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				TableName[DataSize] = '\0';
				TableName[DataSize + 1] = '\0';

				SQLTCHAR ColumnName[Global::MAXNUM_COLUMNS][Global::COLUMNNAME_LENGTH];
				SQLTCHAR ColumnType[Global::MAXNUM_COLUMNS][Global::COLUMNTYPE_LENGTH];
				SQLTCHAR IsNull[Global::MAXNUM_COLUMNS][10];
				SQLTCHAR StateMsg[10];
				SQLTCHAR Msg[1024];

				TCHAR ConnStr[256];
				int Init;
				int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
				DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
				int NumOfCols = Da->GetColumnInfoByTableName((SQLTCHAR*)TableName, ColumnName, ColumnType, IsNull, StateMsg, Msg, 1024);

				TCHAR Data[Global::MAX_PARAM_LENGTH];
				_snwprintf_s(Data, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("["));
				for (int Loop = 0; Loop < NumOfCols; Loop++) {
					TCHAR TmpData[Global::MAX_PARAM_LENGTH];

					TCHAR ColumnNameTmpV[Global::COLUMNNAME_LENGTH * 5];
					TCHAR ColumnNameTmp[Global::COLUMNNAME_LENGTH * 5];
					StkGeneric::GetInstance()->HtmlEncode(ColumnName[Loop], ColumnNameTmpV, Global::COLUMNNAME_LENGTH * 5);
					StkGeneric::GetInstance()->JsonEncode(ColumnNameTmpV, ColumnNameTmp, Global::COLUMNNAME_LENGTH * 5);

					TCHAR ColumnTypeTmp[Global::COLUMNTYPE_LENGTH];
					StkGeneric::GetInstance()->JsonEncode(ColumnType[Loop], ColumnTypeTmp, Global::COLUMNTYPE_LENGTH);

					TCHAR ColTypeCnv[Global::COLUMNTYPE_LENGTH];
					Da->ConvertAttrType(ColumnType[Loop], ColTypeCnv);
					_snwprintf_s(TmpData, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("{ \"title\": \"%s\", \"width\": 100, \"dataType\": \"%s\", \"dataIndx\": \"%d\", \"coltype\": \"%s\", \"isnull\": \"%s\" }"), ColumnNameTmp, ColTypeCnv, Loop, ColumnTypeTmp, IsNull[Loop]);
					lstrcat(Data, TmpData);
					if (Loop + 1 < NumOfCols) {
						lstrcat(Data, _T(","));
					}
				}
				lstrcat(Data, _T("]"));
				StkSocket_Send(Id, Id, (BYTE*)Data, Global::MAX_PARAM_LENGTH);

				OdbcManager::GetInstance()->DeleteAccessorObject(Da);
				delete TableName;
			}

			///// GetTblns /////
			if (memcmp(Command, (char*)"[GetTblns]", 10) == 0) {
				TCHAR Data[Global::MAX_PARAM_LENGTH];
				SQLTCHAR StateMsg[10];
				SQLTCHAR Msg[1024];

				TCHAR ConnStr[256];
				int Init;
				int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
				DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
				Da->GetTables(Data, Global::MAX_PARAM_LENGTH, StateMsg, Msg, 1024);
				OdbcManager::GetInstance()->DeleteAccessorObject(Da);
				StkSocket_Send(Id, Id, (BYTE*)Data, (lstrlen(Data) + 1) * sizeof(TCHAR));
			}

			///// LoggingI /////
			if (memcmp(Command, (char*)"[LoggingI]", 10) == 0) {
				BYTE* LogMsg = new BYTE[DataSize + 1];
				int Ret = StkSocket_Receive(Id, Id, LogMsg, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				LogMsg[DataSize] = '\0';
				DataAccess::GetInstance()->AddLogMsg((TCHAR*)LogMsg);
				delete LogMsg;
				StkSocket_Send(Id, Id, (BYTE*)"OK", 2);
			}

			///// LogsHtml /////
			if (memcmp(Command, (char*)"[LogsHtml]", 10) == 0) {
				TCHAR LogAsHtml[Global::MAX_PARAM_LENGTH / 2];
				DataAccess::GetInstance()->GetLogAsHtml(LogAsHtml);
				StkSocket_Send(Id, Id, (BYTE*)LogAsHtml, (lstrlen(LogAsHtml) + 1) * sizeof(TCHAR));
			}

			///// FilterSw /////
			if (memcmp(Command, (char*)"[FilterSw]", 10) == 0) {
				BYTE* Switch = new BYTE[DataSize + 1];
				int Ret = StkSocket_Receive(Id, Id, Switch, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				Switch[DataSize] = '\0';
				int SwitchVal = atoi((char*)Switch);
				DataAccess::GetInstance()->SetFilterSwitch((SwitchVal == 1)? TRUE : FALSE);
				StkSocket_Send(Id, Id, (BYTE*)"OK", 2);
				delete Switch;
			}

			///// GetFilSw /////
			if (memcmp(Command, (char*)"[GetFilSw]", 10) == 0) {
				if (DataAccess::GetInstance()->GetFilterSwitch() == TRUE) {
					StkSocket_Send(Id, Id, (BYTE*)"TRUE", 4);
				} else {
					StkSocket_Send(Id, Id, (BYTE*)"FALSE", 5);
				}
			}

			///// GetFilCd /////
			if (memcmp(Command, (char*)"[GetFilCd]", 10) == 0) {
				TCHAR ColumnName[Global::COLUMNNAME_LENGTH];
				TCHAR ConvColumnNameTmp[Global::COLUMNNAME_LENGTH * 5];
				TCHAR ConvColumnName[Global::COLUMNNAME_LENGTH * 5];
				int FilterOpeType;
				TCHAR Value[Global::COLUMNVAL_LENGTH];
				TCHAR ConvValue[Global::COLUMNVAL_LENGTH * 5];

				TCHAR Data[Global::MAX_PARAM_LENGTH];
				_snwprintf_s(Data, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("["));
				for (int Loop = 1; Loop <= 5; Loop++) {
					DataAccess::GetInstance()->GetFilterCondition(Loop, ColumnName, &FilterOpeType, Value);
					StkGeneric::GetInstance()->JsonEncode(Value, ConvValue, Global::COLUMNVAL_LENGTH * 5);
					StkGeneric::GetInstance()->HtmlEncode(ColumnName, ConvColumnNameTmp, Global::COLUMNNAME_LENGTH * 5);
					StkGeneric::GetInstance()->JsonEncode(ConvColumnNameTmp, ConvColumnName, Global::COLUMNNAME_LENGTH * 5);

					TCHAR TmpData[Global::MAX_PARAM_LENGTH];
					_snwprintf_s(TmpData, Global::MAX_PARAM_LENGTH, _TRUNCATE, _T("{ \"columnname\": \"%s\", \"opetype\": %d, \"value\": \"%s\" }"), ConvColumnName, FilterOpeType, ConvValue);
					lstrcat(Data, TmpData);
					if (Loop + 1 <= 5) {
						lstrcat(Data, _T(","));
					}
				}
				lstrcat(Data, _T("]"));
				StkSocket_Send(Id, Id, (BYTE*)Data, (lstrlen(Data) + 1) * sizeof(TCHAR));
			}

			///// SetFilCd /////
			if (memcmp(Command, (char*)"[SetFilCd]", 10) == 0) {
				BYTE* Data = new BYTE[DataSize];
				StkSocket_Receive(Id, Id, Data, DataSize, 10000000 + DataSize, (BYTE*)NULL, 0, FALSE);
				int Index;
				int LenOfColumnName;
				int OpeType;
				int LenOfValue;
				TCHAR ColumnName[Global::COLUMNNAME_LENGTH];
				TCHAR Value[Global::COLUMNVAL_LENGTH];
				sscanf_s((char*)Data, "%d %d %d %d", &Index, &LenOfColumnName, &OpeType, &LenOfValue);
				BYTE* Ptr = Data;
				while (*Ptr != '\0') {
					Ptr++;
				}
				Ptr++;
				memcpy(ColumnName, Ptr, LenOfColumnName);
				Ptr += LenOfColumnName;
				memcpy(Value, Ptr, LenOfValue);
				DataAccess::GetInstance()->SetFilterCondition(Index, ColumnName, OpeType, Value);
				delete Data;
				if (Index == 5) {
					TCHAR Buf[1024];
					_snwprintf_s(Buf, 1024, _TRUNCATE, _T("%s"), MessageProc::GetMsg(MessageProc::CMDFRK_FILTERINGCOND_UPDATE));
					DataAccess* DatAc = DataAccess::GetInstance();
					DatAc->AddLogMsg(Buf);
				}
			}
			// Logging for Command
			/*{
				TCHAR LogBuf[32];
				_snwprintf_s(LogBuf, 32, _TRUNCATE, _T("%S END"), Command);
				DataAccess::GetInstance()->AddLogMsg((TCHAR*)LogBuf);
			}*/
		}
		StkSocket_CloseAccept(Id, Id, TRUE);
	}
	return 0;
}
