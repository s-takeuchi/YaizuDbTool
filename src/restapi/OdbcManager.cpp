﻿#include "OdbcManager.h"
#include "../../../YaizuComLib/src/stkpl/StkPl.h"
#include "Global.h"
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

DbAccessor* OdbcManager::CreateAccessorObject(int Type)
{
	if (Type == POSTGRESQL_ACCESSOR) {
		return new DbPostgreSqlAccessor();
	}
	if (Type == MARIADB_ACCESSOR) {
		return new DbMariaDbAccessor();
	}
	if (Type == MYSQL_ACCESSOR) {
		return new DbMySqlAccessor();
	}
	return NULL;
}

void OdbcManager::DeleteAccessorObject(DbAccessor* Target)
{
	delete Target;
}
