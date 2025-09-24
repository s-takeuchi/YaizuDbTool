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
// Type [Input] : Type of DBMS. If ommitted, the value in StkData is used.
// Return : Created DbAccessor object
DbAccessor* OdbcManager::CreateAccessorObject(int Type)
{
	int Init;
	wchar_t ConnStr[256];
	int DbmsType = DataAccess::GetInstance()->GetOdbcConfing(ConnStr, &Init);

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
