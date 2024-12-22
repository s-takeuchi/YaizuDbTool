#pragma once
#ifdef WIN32
	#include <windows.h>
#endif
#include "Global.h"
#include "DbAccessor.h"

class DbMySqlAccessor : public DbAccessor
{
private:
	static const int TYPE_KEY = 0;
	static const int TYPE_VALUE = 1;
	static const int TYPE_LIKE_VALUE = 2;

public:
	DbMySqlAccessor();
	virtual ~DbMySqlAccessor();

	virtual void GetDefaultConnStr(wchar_t[Global::MAX_PARAM_LENGTH]);
	virtual int GetNumOfRecords(wchar_t*, wchar_t[10], wchar_t[1024]);
	virtual int GetTables(StkObject*, wchar_t[10], wchar_t[1024]);
	virtual int GetColumnInfoByTableName(wchar_t*,	StkObject*, wchar_t[10], wchar_t[1024]);
	virtual int GetRecordsByTableName(wchar_t*, int, StkObject*, wchar_t*, wchar_t*, int, int, wchar_t[10], wchar_t[1024]);
	virtual int ConvertAttrType(wchar_t[Global::COLUMNTYPE_LENGTH], wchar_t[Global::COLUMNTYPE_LENGTH]);
	virtual void SqlEncoding(wchar_t*, wchar_t*, int);
};
