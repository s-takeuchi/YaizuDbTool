#include "OdbcManager.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "dataaccess.h"
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

// Create DbAccessor object
// Return : Created DbAccessor object
DbAccessor* OdbcManager::CreateAccessorObject()
{
	wchar_t ConnStr[256];
	int Init;
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);

	if (DbmsType == POSTGRESQL_ACCESSOR) {
		return new DbPostgreSqlAccessor(ConnStr);
	}
	if (DbmsType == MARIADB_ACCESSOR) {
		return new DbMariaDbAccessor(ConnStr);
	}
	if (DbmsType == MYSQL_ACCESSOR) {
		return new DbMySqlAccessor(ConnStr);
	}
	return NULL;
}

void OdbcManager::DeleteAccessorObject(DbAccessor* Target)
{
	delete Target;
}
