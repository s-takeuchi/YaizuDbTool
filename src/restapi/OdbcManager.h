#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "..\Global.h"
#include "DbAccessor.h"

class OdbcManager
{
private:
	// This instance
	static OdbcManager* ThisInstance;
	OdbcManager();

public:
	static const int MARIADB_ACCESSOR = 0;
	static const int POSTGRESQL_ACCESSOR = 1;
	static const int MYSQL_ACCESSOR = 2;

public:
	~OdbcManager();

	// Get this instance
	static OdbcManager* GetInstance();

	// Create accessor object
	DbAccessor* CreateAccessorObject(int);
	// Delete accessor object
	void DeleteAccessorObject(DbAccessor*);

};
