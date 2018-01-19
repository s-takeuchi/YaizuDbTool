#pragma once
#include <windows.h>
#include <tchar.h>

class Global
{
public:
	static const int MAX_HTTP_REQUEST_SIZE = 28800;
	static const int CMDFREAK_SERVER_NAME = 256;
	static const int MAX_PARAM_LENGTH = 28800;

	static const int TABLENAME_LENGTH = 65;
	static const int COLUMNNAME_LENGTH = 65;
	static const int COLUMNTYPE_LENGTH = 64;
	static const int COLUMNVAL_LENGTH = 64;

	static const int MAXNUM_TABLES = 300;
	static const int MAXNUM_COLUMNS = 50;
	static const int MAXNUM_RECORDS = 15000;

	static const int MAXLEN_OF_LOGTIME = 32;
	static const int MAXLEN_OF_LOGMSG = 100;
	static const int MAXNUM_OF_LOGRECORDS = 111;
};
