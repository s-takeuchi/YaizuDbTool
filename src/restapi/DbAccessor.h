#pragma once
#ifdef WIN32
    #include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include "../../../YaizuComLib/src/commonfunc/StkObject.h"
#include "Global.h"

class DbAccessor
{
protected:
	SQLHENV  Henv;
	SQLHDBC  Hdbc;
	SQLHSTMT Hstmt;

protected:
	SQLRETURN OpenDatabase(wchar_t*, wchar_t[10], wchar_t[1024]);
	SQLRETURN CloseDatabase(wchar_t[10], wchar_t[1024]);

	SQLRETURN GetTablesCommon(wchar_t*, StkObject*, wchar_t[10], wchar_t[1024]);

	virtual int GetRecordsByTableNameCommon(wchar_t*, int, StkObject* RecObj,
		wchar_t[5][Global::COLUMNNAME_LENGTH * 4 + 2], int[5], wchar_t[5][Global::COLUMNVAL_LENGTH * 4 + 2],
		wchar_t[Global::COLUMNNAME_LENGTH * 4 + 2], wchar_t[5],
		int, int,
		wchar_t[10], wchar_t[1024]);

	void ConvertMessage(wchar_t[10], wchar_t[1024], const SQLTCHAR[10], const SQLTCHAR[1024]);

public:
	DbAccessor();
	virtual ~DbAccessor();

	virtual int Test(wchar_t[Global::MAX_PARAM_LENGTH], wchar_t[1024]);

	virtual int GetNumOfRecords(wchar_t*, wchar_t[10], wchar_t[1024]) = 0;
	int GetNumOfRecordsCommon(wchar_t*, wchar_t[5][Global::COLUMNNAME_LENGTH * 4 + 2], int[5], wchar_t[5][Global::COLUMNVAL_LENGTH * 4 + 2], wchar_t[10], wchar_t[1024]);

	virtual SQLRETURN GetTables(StkObject*, wchar_t[10], wchar_t[1024]) = 0;

	virtual int GetColumnInfoByTableName(wchar_t*,	StkObject*, wchar_t[10], wchar_t[1024]) = 0;

	virtual int GetRecordsByTableName(wchar_t*, int, StkObject*, wchar_t*, wchar_t*, int, int, wchar_t[10], wchar_t[1024]) = 0;

	virtual int ConvertAttrType(SQLTCHAR[Global::COLUMNTYPE_LENGTH], SQLTCHAR[Global::COLUMNTYPE_LENGTH]) = 0;

	virtual void GetDefaultConnStr(wchar_t[Global::MAX_PARAM_LENGTH]) = 0;

	virtual void SqlEncoding(wchar_t*, wchar_t*, int) = 0;
};
