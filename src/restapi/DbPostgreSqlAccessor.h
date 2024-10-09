#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "../Global.h"
#include "DbAccessor.h"

class DbPostgreSqlAccessor : public DbAccessor
{
private:
	static const int TYPE_KEY = 0;
	static const int TYPE_VALUE = 1;
	static const int TYPE_LIKE_VALUE = 2;

public:
	DbPostgreSqlAccessor();
	virtual ~DbPostgreSqlAccessor();

	virtual void GetDefaultConnStr(SQLTCHAR[Global::MAX_PARAM_LENGTH]);
	virtual int GetNumOfRecords(SQLTCHAR*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);
	virtual SQLRETURN GetTables(StkObject*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);
	virtual int GetColumnInfoByTableName(SQLTCHAR*,	StkObject*, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);
	virtual int GetRecordsByTableName(SQLTCHAR*, int, StkObject*, SQLTCHAR*, SQLTCHAR*, int, int, SQLTCHAR[10], SQLTCHAR*, SQLSMALLINT);
	virtual int ConvertAttrType(SQLTCHAR[Global::COLUMNTYPE_LENGTH], SQLTCHAR[Global::COLUMNTYPE_LENGTH]);
	virtual void SqlEncoding(SQLTCHAR*, SQLTCHAR*, int);
};
