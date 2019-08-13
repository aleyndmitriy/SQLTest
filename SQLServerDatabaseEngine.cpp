#include"SQLServerDatabaseEngine.h"

bool DrvFtaeAlarm::SQLServerDatabaseEngine::OpenConnection()
{
	if (!environment) {
		environment = std::make_shared<SQLServerEnvironment>();
	}

	connection = std::make_shared<SQLServerConnection>(environment);
	bool isOpen = connection->IsValidConnection();
	return isOpen;
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::OpenConnection(const ConnectionAttributes& attributes)
{
	if (!environment) {
		environment = std::make_shared<SQLServerEnvironment>();
	}
	
	connection = std::make_shared<SQLServerConnection>(environment,attributes);
	if (connection->IsValidConnection() == false) {
		return false;
	}
	ConnectionAttributes attr = connection->GetConnectionAttributes();
	if (attr != attributes) {
		return false;
	}
	return true;
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::OpenConnectionIfNeeded(const ConnectionAttributes& attributes)
{
	bool isValid = false;
	if (connection) {
		ConnectionAttributes attr = connection->GetConnectionAttributes();
		ConnectionStatus status = connection->GetConnectionStatus();
		switch (status)
		{
		case ConnectionStatus::ConnectToDatabase:
			if (attributes == attr) {
				return true;
			}
			else {
				CloseConnection();
				return OpenConnection(attributes);
			}
			break;
		case ConnectionStatus::ConnectToServer:
			if (attributes.serverName == attr.serverName) {
				if (!attributes.databaseName.empty()) {
					return ChooseDatabase(attributes.databaseName);
				}
				else {
					return OpenConnection(attributes);
				}
				return true;
			}
			else {
				CloseConnection();
				return OpenConnection(attributes);
			}
			break;
		case ConnectionStatus::ConnectToDriver:
			if (attributes.driver == attr.driver) {
				if (!attributes.serverName.empty()) {
					AuthenticationType authtype = AuthenticationType::System;
					if (attributes.isServerAuthentication) {
						authtype = AuthenticationType::Server;
					}
					isValid = loadDatabaseInstances(attributes.serverName, authtype, attributes.loginName, attributes.password);
					if (isValid) {
						if (attributes.databaseName.empty()) {
							return isValid;
						}
						else {
							return ChooseDatabase(attributes.databaseName);
						}
					}
					else {
						return false;
					}
				}
			}
			else {
				CloseConnection();
				return OpenConnection(attributes);
			}
		default:
			CloseConnection();
			return OpenConnection(attributes);
			break;
		}
	}
	else {
		CloseConnection();
		return OpenConnection(attributes);
	}
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::loadServerInstances(std::string driverName)
{
	bool isConnect = connection->ConnectToServerInstances(driverName);
	return isConnect;
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::loadDatabaseInstances(std::string serverName, AuthenticationType type, std::string user, std::string password)
{
	bool isConnect = connection->ConnectToDatabaseInstances(serverName, user, password, type);
	return isConnect;
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::ChooseDatabase(std::string databaseName)
{
	if (connection->ConnectToDatabase(databaseName)) {
		return true;
	}
	return false;
}

void DrvFtaeAlarm::SQLServerDatabaseEngine::CloseConnection()
{
	connection.reset();
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::IsValidConnection() const
{
	if (connection) {
		return connection->IsValidConnection();
	}
	else {
		return false;
	}
}

DrvFtaeAlarm::ConnectionAttributes DrvFtaeAlarm::SQLServerDatabaseEngine::GetConnectionAttributes() const
{
	if (connection) {
		return connection->GetConnectionAttributes();
	}
	else {
		return ConnectionAttributes();
	}
}

DrvFtaeAlarm::ConnectionStatus DrvFtaeAlarm::SQLServerDatabaseEngine::GetConnectionStatus() const {
	if (connection) {
		return connection->GetConnectionStatus();
	}
	else {
		return ConnectionStatus::NoConnect;
	}
}

DrvFtaeAlarm::SQLServerDatabaseEngine::~SQLServerDatabaseEngine()
{
	CloseConnection();
	environment.reset();
}

std::vector<std::string> DrvFtaeAlarm::SQLServerDatabaseEngine::GetServersList() const
{
	return connection->GetServerList();
}
std::vector<std::string> DrvFtaeAlarm::SQLServerDatabaseEngine::GetDatabasesList() const
{
	return connection->GetDatabaseList();
}

std::vector<DrvFtaeAlarm::Record> DrvFtaeAlarm::SQLServerDatabaseEngine::ExecuteStatement(const std::string& query, const std::vector<std::string>& parameters)
{
	std::unique_ptr<SQLServerStatement> stm = std::make_unique<SQLServerStatement>(connection, query, parameters);
	std::vector<DrvFtaeAlarm::Record> vec = stm->Execute();
	return vec;
}

