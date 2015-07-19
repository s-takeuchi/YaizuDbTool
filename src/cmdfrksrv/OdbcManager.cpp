#include <windows.h>
#include "OdbcManager.h"
#include "..\Global.h"
#include "DbAccessor.h"
#include "PostgreSqlAccessor.h"
#include "MariaDbAccessor.h"

OdbcManager* OdbcManager::ThisInstance;

OdbcManager::OdbcManager()
{
}

OdbcManager::~OdbcManager()
{
}

// Get this instance
OdbcManager* OdbcManager::GetInstance()
{
	static int Init = 1;
	if (Init == 1) {
		ThisInstance = new OdbcManager();
		Init = 0;
	}
	return ThisInstance;
}

DbAccessor* OdbcManager::CreateAccessorObject(int Type)
{
	if (Type == POSTGRESQL_ACCESSOR) {
		return new PostgreSqlAccessor();
	}
	if (Type ==MARIADB_ACCESSOR) {
		return new MariaDbAccessor();
	}
	return NULL;
}

void OdbcManager::DeleteAccessorObject(DbAccessor* Target)
{
	delete Target;
}
