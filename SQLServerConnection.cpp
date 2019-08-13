#include"SQLServerConnection.h"
#include"Constants.h"
#include "Utils.h"
#include<tchar.h>
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
	connectionAttributes.driver.clear();
	connectionAttributes.serverName.clear();
	connectionAttributes.loginName.clear();
	connectionAttributes.password.clear();
	connectionAttributes.databaseName.clear();
	connectionAttributes.isSystemAuthentication = false;
}

void DrvFtaeAlarm::SQLServerConnection::allocateConnection() {
	SQLSMALLINT res = SQLAllocHandle(SQL_HANDLE_DBC, ptrEnvironment->GetInterface(), &sqlDBC);
	if (res == SQL_ERROR)
	{
		HandleDiagnosticRecord();
		freeConnection();
		return;
	}
	bool isConnect = false;
	if (connectionAttributes.driver.empty()) {
		return;
	}
	isConnect = ConnectToServerInstances(connectionAttributes.driver);
	if (serverList.empty() || !isConnect) {
		return;
	}
	if (connectionAttributes.serverName.empty() || connectionAttributes.loginName.empty() || connectionAttributes.password.empty()) {
		return;
	}
	AuthenticationType authtype = AuthenticationType::Server;
	if (connectionAttributes.isSystemAuthentication) {
		authtype = AuthenticationType::System;
	}
	isConnect = ConnectToDatabaseInstances(connectionAttributes.serverName, connectionAttributes.loginName, connectionAttributes.password, authtype);
	if (databaseList.empty() || !isConnect) {
		return;
	}
	if (connectionAttributes.databaseName.empty()) {
		return;
	}
	ConnectToDatabase(connectionAttributes.databaseName);
}

bool DrvFtaeAlarm::SQLServerConnection::ConnectToServerInstances(std::string driverName) {
	
	TCHAR wchDriverName[MAX_DRIVERNAME_LENGTH];
	TCHAR wStrOut[STR_LENGTH];
	SQLSMALLINT shBrowseResultLen = 0;
	std::string strDriver = std::string("DRIVER={") + driverName + std::string("};");
	StringCchCopy(wchDriverName, strDriver.length() + 1, strDriver.c_str());
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
				return false;
			}
			else {
				serverList.assign(servers.begin(), servers.end());
				return true;
			}
		}
		else {
			freeConnection();
			return false;
		}
	}
	else {
		HandleDiagnosticRecord();
		freeConnection();
		return false;
	}
}

bool DrvFtaeAlarm::SQLServerConnection::ConnectToDatabaseInstances(std::string serverName, std::string login, std::string password, AuthenticationType authType)
{
	TCHAR whUserInfo[SQL_MAX_MESSAGE_LENGTH];
	TCHAR wStrOut[STR_LENGTH];
	SQLSMALLINT shBrowseResultLen = 0;
	std::string strServerData = std::string("SERVER=") + serverName;
	if (authType == AuthenticationType::Server) {
		strServerData = strServerData + std::string(";UID=") + login + std::string(";PWD=") + password;
	}
	else {
		strServerData = strServerData + std::string(";Trusted_Connection=yes");
	}
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
				return true;
			}
			else {
				databaseList.assign(databases.begin(), databases.end());
				connectionAttributes.serverName = serverName;
				if (authType == AuthenticationType::System) {
					connectionAttributes.isSystemAuthentication = true;
				}
				else {
					connectionAttributes.isSystemAuthentication = false;
				}
				return true;
			}
		}
		else {
			freeConnection();
			return false;
		}
	}
	else {
		HandleDiagnosticRecord();
		freeConnection();
		return false;
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
		HandleDiagnosticRecord();
		freeConnection();
		return false;
	}
	connectionAttributes.databaseName = databaseName;
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

bool DrvFtaeAlarm::SQLServerConnection::IsValidConnection() const
{
	return (sqlDBC != SQL_NULL_HDBC);
}

DrvFtaeAlarm::ConnectionStatus DrvFtaeAlarm::SQLServerConnection::GetConnectionStatus() const
{
	if (!IsValidConnection()) {
		return ConnectionStatus::NoConnect;
	}

	if (!connectionAttributes.databaseName.empty()) {
		return ConnectionStatus::ConnectToDatabase;
	}
	else if (!connectionAttributes.serverName.empty()) {
		return ConnectionStatus::ConnectToServer;
	}
	else if (!connectionAttributes.driver.empty()) {
		return ConnectionStatus::ConnectToDriver;
	}
	else {
		return ConnectionStatus::NoConnect;
	}
}

void DrvFtaeAlarm::SQLServerConnection::HandleDiagnosticRecord()
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	TCHAR       wszMessage[SQL_MAX_MESSAGE_LENGTH];
	TCHAR       wszState[SQL_SQLSTATE_SIZE + 1];
	while (SQLGetDiagRec(SQL_HANDLE_DBC, sqlDBC, ++iRec, reinterpret_cast<SQLCHAR*>(wszState), &iError, reinterpret_cast<SQLCHAR*>(wszMessage),
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(TCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS)
	{
		if (_tcsncmp(wszState, "01004", 5))
		{
			fprintf(stderr, "[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}