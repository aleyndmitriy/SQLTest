#include"SQLServerEnvironment.h"

DrvFtaeAlarm::SQLServerEnvironment::SQLServerEnvironment() :sqlEnvironment(SQL_NULL_HENV) {
	SQLSMALLINT res = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvironment);

	res = SQLSetEnvAttr(sqlEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
}

DrvFtaeAlarm::SQLServerEnvironment::~SQLServerEnvironment() {
	SQLSMALLINT res = SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvironment);
}

void* DrvFtaeAlarm::SQLServerEnvironment::GetInterface() {
	return sqlEnvironment;
}