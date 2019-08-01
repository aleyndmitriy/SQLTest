#include"SQLServerConnection.h"
#include"Constants.h"
#include "Utils.h"

DrvFtaeAlarm::SQLServerConnection::SQLServerConnection(const std::shared_ptr<SQLServerEnvironment>& environment, const ConnectionAttributes& attributes) :Connection(environment, attributes), sqlDBC(SQL_NULL_HDBC)
{
	allocateConnection();
}

DrvFtaeAlarm::SQLServerConnection::SQLServerConnection(const std::shared_ptr<SQLServerEnvironment>& environment) : Connection(environment), sqlDBC(SQL_NULL_HDBC)
{
	allocateConnection();
}

DrvFtaeAlarm::SQLServerConnection::SQLServerConnection(std::shared_ptr<SQLServerEnvironment>&& environment, ConnectionAttributes&& attributes) : Connection(std::move(environment), std::move(attributes)), sqlDBC(SQL_NULL_HDBC)
{
	allocateConnection();
}

DrvFtaeAlarm::SQLServerConnection::SQLServerConnection(std::shared_ptr<SQLServerEnvironment>&& environment) : Connection(std::move(environment)), sqlDBC(SQL_NULL_HDBC)
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
	databaseList.clear();
	serverList.clear();
}

void DrvFtaeAlarm::SQLServerConnection::allocateConnection() {
	SQLSMALLINT res = SQLAllocHandle(SQL_HANDLE_DBC, ptrEnvironment->GetInterface(), &sqlDBC);
	if (res == SQL_ERROR)
	{
		freeConnection();
		return;
	}
	if (connectionAttributes.driver.empty()) {
		return;
	}
	ConnectToServerInstances(connectionAttributes.driver);
	if (serverList.empty()) {
		return;
	}
	if (connectionAttributes.serverName.empty() || connectionAttributes.loginName.empty() || connectionAttributes.password.empty()) {
		return;
	}
	ConnectToDatabaseInstances(connectionAttributes.serverName, connectionAttributes.loginName, connectionAttributes.password);
	if (databaseList.empty()) {
		return;
	}
	if (connectionAttributes.databaseName.empty()) {
		return;
	}
	ConnectToDatabase(connectionAttributes.databaseName);
}

void DrvFtaeAlarm::SQLServerConnection::ConnectToServerInstances(std::string driverName) {
	
	TCHAR wchDriverName[MAX_DRIVERNAME_LENGTH];
	TCHAR wStrOut[STR_LENGTH];
	SQLSMALLINT shBrowseResultLen = 0;
	std::string strDriver = std::string("DRIVER={") + driverName + std::string("};");
	std::wstring wDrwName = Str2Wstr(strDriver);
	StringCchCopy(wchDriverName, wDrwName.length() + 1, strDriver.c_str());
	SQLSMALLINT res = SQLBrowseConnect(sqlDBC, reinterpret_cast<SQLCHAR*>(wchDriverName), SQL_NTS,
		reinterpret_cast<SQLCHAR*>(wStrOut), STR_LENGTH, &shBrowseResultLen);
	if (SQL_SUCCEEDED(res) || res == SQL_NEED_DATA) {
		std::string strRes = std::string(wStrOut);
		size_t firstPos = strRes.find_first_of("{",0);
		size_t lastPos = strRes.find_first_of("}", 0);
		if (firstPos != std::string::npos && lastPos != std::string::npos) {
			std::string serversString = strRes.substr(firstPos + 1, lastPos - firstPos - 1);
			std::vector<std::string> servers = split(serversString, std::string(","));
			if (servers.empty()) {
				freeConnection();
			}
			else {
				serverList.assign(servers.begin(), servers.end());
			}
		}
		else {
			freeConnection();
		}
	}
	else {
		freeConnection();
	}
}

void DrvFtaeAlarm::SQLServerConnection::ConnectToDatabaseInstances(std::string serverName, std::string login, std::string password)
{
	TCHAR whUserInfo[SQL_MAX_MESSAGE_LENGTH];
	TCHAR wStrOut[STR_LENGTH];
	SQLSMALLINT shBrowseResultLen = 0;
	std::string strServerData = std::string("SERVER=") + serverName + std::string(";UID=") + login + std::string(";PWD=") + password;
	StringCchCopy(whUserInfo, strServerData.length() + 1, strServerData.c_str());
	SQLSMALLINT res = SQLBrowseConnect(sqlDBC, reinterpret_cast<SQLCHAR*>(whUserInfo), SQL_NTS,
		reinterpret_cast<SQLCHAR*>(wStrOut), STR_LENGTH, &shBrowseResultLen);
	if (SQL_SUCCEEDED(res) || res == SQL_NEED_DATA) {
		std::string strRes(wStrOut);
		size_t firstPos = strRes.find_first_of("{", 0);
		size_t lastPos = strRes.find_first_of("}", 0);
		if (firstPos != std::string::npos && lastPos != std::string::npos) {
			std::string databaseString = strRes.substr(firstPos + 1, lastPos - firstPos - 1);
			std::vector<std::string> databases = split(databaseString, std::string(","));
			if (databases.empty()) {
				freeConnection();
			}
			else {
				databaseList.assign(databases.begin(), databases.end());
			}
		}
		else {
			freeConnection();
		}
	}
	else {
		freeConnection();
	}
}

bool DrvFtaeAlarm::SQLServerConnection::ConnectToDatabase(std::string databaseName)
{
	TCHAR wchDataBaseName[MAX_DATABASENAME_LENGTH];
	TCHAR wStrOut[STR_LENGTH];
	SQLSMALLINT shBrowseResultLen = 0;
	std::string strDatabase = std::string("DATABASE=") + databaseName + std::string(";");
	StringCchCopy(wchDataBaseName, strDatabase.length() + 1, strDatabase.c_str());
	SQLSMALLINT res = SQLBrowseConnect(sqlDBC, reinterpret_cast<SQLCHAR*>(wchDataBaseName), SQL_NTS,
		reinterpret_cast<SQLCHAR*>(wStrOut), STR_LENGTH, &shBrowseResultLen);
	if (!SQL_SUCCEEDED(res)) {
		freeConnection();
		return false;
	}
	return true;
}

std::vector<std::string> DrvFtaeAlarm::SQLServerConnection::GetServerList() const
{
	return serverList;
}

std::vector<std::string> DrvFtaeAlarm::SQLServerConnection::GetDatabaseList() const
{
	return databaseList;
}