#include"SQLServerConnection.h"
#include"Constants.h"

DrvFtaeAlarm::SQLServerConnection::SQLServerConnection(const std::shared_ptr<SQLServerEnvironment>& environment, const ConnectionAttributes& attributes) :Connection(environment, attributes), sqlDBC(SQL_NULL_HDBC)
{
	allocateConnection();
}

DrvFtaeAlarm::SQLServerConnection::SQLServerConnection(std::shared_ptr<SQLServerEnvironment>&& environment, ConnectionAttributes&& attributes) : Connection(std::move(environment), std::move(attributes)), sqlDBC(SQL_NULL_HDBC)
{
	allocateConnection();
}

DrvFtaeAlarm::SQLServerConnection::~SQLServerConnection() {
	if (sqlDBC)
	{
		freeConnection();
	}
}

void* DrvFtaeAlarm::SQLServerConnection::GetInterface() {
	return sqlDBC;
}

void DrvFtaeAlarm::SQLServerConnection::freeConnection() {
	SQLSMALLINT res = SQLDisconnect(sqlDBC);
	res = SQLFreeHandle(SQL_HANDLE_DBC, sqlDBC);
	sqlDBC = SQL_NULL_HDBC;
	ptrEnvironment.reset();
}

void DrvFtaeAlarm::SQLServerConnection::allocateConnection() {
	SQLSMALLINT res = SQLAllocHandle(SQL_HANDLE_DBC, ptrEnvironment->GetInterface(), &sqlDBC);
	if (res == SQL_ERROR)
	{
		freeConnection();
		return;
	}
	SQLSMALLINT shBrowseResultLen = 0;
	char chDriverName[MAX_DRIVERNAME_LENGTH];
	int numberOfCharachters = sprintf_s(chDriverName, MAX_DRIVERNAME_LENGTH, "DRIVER={%s};", connectionAttributes.driver.c_str());
	if (numberOfCharachters < 0)
	{
		freeConnection();
		return;
	}
	wchar_t wchDriverName[MAX_DRIVERNAME_LENGTH];
	numberOfCharachters = MultiByteToWideChar(CP_ACP, 0, chDriverName, -1, wchDriverName, numberOfCharachters);
	res = SQLBrowseConnect(sqlDBC, reinterpret_cast<SQLWCHAR*>(wchDriverName), SQL_NTS,
		NULL, 0, &shBrowseResultLen);
	if (!SQL_SUCCEEDED(res)) {
		freeConnection();
		return;
	}
	char chUserInfo[SQL_MAX_MESSAGE_LENGTH];
	numberOfCharachters = sprintf_s(chUserInfo, SQL_MAX_MESSAGE_LENGTH, "Server=%s;UID=%s;PWD=%s;", connectionAttributes.serverName.c_str(), connectionAttributes.loginName.c_str(), connectionAttributes.password.c_str());
	if (numberOfCharachters < 0)
	{
		freeConnection();
		return;
	}
	wchar_t wchUserInfo[SQL_MAX_MESSAGE_LENGTH];
	numberOfCharachters = MultiByteToWideChar(CP_ACP, 0, chUserInfo, -1, wchUserInfo, numberOfCharachters);
	res = SQLBrowseConnect(sqlDBC, reinterpret_cast<SQLWCHAR*>(wchUserInfo), SQL_NTS,
		NULL, 0, &shBrowseResultLen);
	if (!SQL_SUCCEEDED(res)) {
		freeConnection();
		return;
	}
	char chDataBaseName[MAX_DATABASENAME_LENGTH];
	memset(chDataBaseName, '\0', MAX_DATABASENAME_LENGTH);
	numberOfCharachters = sprintf_s(chDataBaseName, MAX_DATABASENAME_LENGTH, "DATABASE={%s};", connectionAttributes.databaseName.c_str());
	if (numberOfCharachters < 0)
	{
		freeConnection();
		return;
	}
	wchar_t wchDataBaseName[MAX_DATABASENAME_LENGTH];
	numberOfCharachters = MultiByteToWideChar(CP_ACP, 0, chDataBaseName, -1, wchDataBaseName, numberOfCharachters);
	res = SQLBrowseConnect(sqlDBC, reinterpret_cast<SQLWCHAR*>(wchUserInfo), SQL_NTS,
		NULL, 0, &shBrowseResultLen);
	if (!SQL_SUCCEEDED(res)) {
		freeConnection();
	}
}