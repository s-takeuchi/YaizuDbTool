#include "OdbcManager.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "DbAccessor.h"
#include "DbPostgreSqlAccessor.h"
#include "DbMariaDbAccessor.h"
#include "DbMySqlAccessor.h"

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

DbAccessor* OdbcManager::CreateAccessorObject(int Type, wchar_t* ConnStr)
{
	if (Type == POSTGRESQL_ACCESSOR) {
		return new DbPostgreSqlAccessor(ConnStr);
	}
	if (Type == MARIADB_ACCESSOR) {
		return new DbMariaDbAccessor(ConnStr);
	}
	if (Type == MYSQL_ACCESSOR) {
		return new DbMySqlAccessor(ConnStr);
	}
	return NULL;
}

void OdbcManager::DeleteAccessorObject(DbAccessor* Target)
{
	delete Target;
}
