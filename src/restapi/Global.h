#pragma once
#define SERVICE_VERSION "1.3.0"

class Global
{
public:
	static const int MAX_PARAM_LENGTH = 28800;

	static const int DBNAME_LENGTH = 65;
	static const int TABLENAME_LENGTH = 65;

	static const int COLUMNNAME_LENGTH = 65;
	static const int COLUMNVAL_LENGTH = 64;

	static const int MAXLEN_OF_LOGTIME = 32;
	static const int MAXLEN_OF_LOGMSG = 100;
	static const int MAXNUM_OF_LOGRECORDS = 111;

	static const int MAXLEN_OF_USERNAME = 32;
	static const int MAXLEN_OF_PASSWORD = 32;
	static const int MAXNUM_OF_USERRECORDS = 64;

	static void EventLogging(wchar_t*, wchar_t*, int);
};
