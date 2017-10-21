#include <tchar.h>
#include "ApiGetTableInfo.h"
#include "dataaccess.h"
#include "DbAccessor.h"
#include "OdbcManager.h"

StkObject* ApiGetTableInfo::Execute(StkObject* ReqObj, int Method, TCHAR UrlPath[128], int* ResultCode, TCHAR Locale[3])
{
	StkObject* ResObj = new StkObject(_T(""));
	SQLTCHAR StateMsg[10];
	SQLTCHAR Msg[1024];

	AddCodeAndMsg(ResObj, 0, _T(""), _T(""));
	StkObject* DatObj = new StkObject(_T("Data"));

	TCHAR ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);
	DbAccessor* Da = OdbcManager::GetInstance()->CreateAccessorObject(DbmsType);
	Da->GetTables(DatObj, StateMsg, Msg, 1024);

	StkObject* TblObj = DatObj->GetFirstChildElement();
	while (TblObj) {
		if (lstrcmp(TblObj->GetName(), _T("TableInfo")) == 0) {
			StkObject* TblNameObj = TblObj->GetFirstChildElement();
			while (TblNameObj) {
				if (lstrcmp(TblNameObj->GetName(), _T("Name")) == 0) {
					Da->GetColumnInfoByTableName(TblNameObj->GetStringValue(), TblObj, StateMsg, Msg, 1024);
				}
				TblNameObj = TblNameObj->GetNext();
			}
		};
		TblObj = TblObj->GetNext();
	}

	OdbcManager::GetInstance()->DeleteAccessorObject(Da);
	ResObj->AppendChildElement(DatObj);

	*ResultCode = 200;

	return ResObj;
}
