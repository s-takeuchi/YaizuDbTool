#pragma once
#ifdef WIN32
	#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include "Global.h"
#include "DbAccessor.h"

class DbMariaDbAccessor : public DbAccessor
{
private:
	static const int TYPE_KEY = 0;
	static const int TYPE_VALUE = 1;
	static const int TYPE_LIKE_VALUE = 2;

public:
	DbMariaDbAccessor();
	virtual ~DbMariaDbAccessor();

	virtual void GetDefaultConnStr(wchar_t[Global::MAX_PARAM_LENGTH]);
	virtual int GetNumOfRecords(wchar_t*, wchar_t[10], wchar_t[1024]);
	virtual SQLRETURN GetTables(StkObject*, wchar_t[10], wchar_t[1024]);
	virtual int GetColumnInfoByTableName(wchar_t*,	StkObject*, wchar_t[10], wchar_t[1024]);
	virtual int GetRecordsByTableName(SQLTCHAR*, int, StkObject*, SQLTCHAR*, SQLTCHAR*, int, int, wchar_t[10], wchar_t[1024]);
	virtual int ConvertAttrType(SQLTCHAR[Global::COLUMNTYPE_LENGTH], SQLTCHAR[Global::COLUMNTYPE_LENGTH]);
	virtual void SqlEncoding(wchar_t*, wchar_t*, int);
};
