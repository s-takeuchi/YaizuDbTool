#pragma once
#include <windows.h>
#include <tchar.h>
#include <sql.h>
#include <sqlext.h>
#include "..\..\..\YaizuComLib\src\commonfunc\StkObject.h"
#include "..\Global.h"

class DbAccessor
{
protected:
	SQLHENV  Henv;
	SQLHDBC  Hdbc;
	SQLHSTMT Hstmt;

protected:
	SQLRETURN OpenDatabase(SQLTCHAR*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);
	SQLRETURN CloseDatabase(SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);

	SQLRETURN GetTablesCommon(SQLTCHAR*, StkObject*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);

	virtual int GetRecordsByTableNameCommon(SQLTCHAR*, int, StkObject* RecObj,
		TCHAR[5][Global::COLUMNNAME_LENGTH * 4 + 2], int[5], TCHAR[5][Global::COLUMNVAL_LENGTH * 4 + 2],
		SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);


public:
	DbAccessor();
	virtual ~DbAccessor();

	virtual int Test(TCHAR[Global::MAX_PARAM_LENGTH]);

	virtual int GetNumOfRecords(SQLTCHAR*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT) = 0;
	int GetNumOfRecordsCommon(SQLTCHAR*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);

	virtual SQLRETURN GetTables(StkObject*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT) = 0;

	virtual int GetColumnInfoByTableName(SQLTCHAR*,	StkObject*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT) = 0;

	virtual int GetRecordsByTableName(SQLTCHAR*, int, StkObject*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT) = 0;

	virtual int ConvertAttrType(SQLTCHAR[Global::COLUMNTYPE_LENGTH], SQLTCHAR[Global::COLUMNTYPE_LENGTH]) = 0;

	virtual void GetDefaultConnStr(SQLTCHAR[Global::MAX_PARAM_LENGTH]) = 0;

	virtual void SqlEncoding(SQLTCHAR*, SQLTCHAR*, int) = 0;

};
