#include"SQLServerDatabaseEngine.h"

void DrvFtaeAlarm::SQLServerDatabaseEngine::OpenConnection()
{
	if (!environment) {
		environment = std::make_shared<SQLServerEnvironment>();
	}
	connection = std::make_shared<SQLServerConnection>(environment);
}

void DrvFtaeAlarm::SQLServerDatabaseEngine::OpenConnection(const ConnectionAttributes& attributes)
{
	if (!environment) {
		environment = std::make_shared<SQLServerEnvironment>();
	}
	connection = std::make_shared<SQLServerConnection>(environment,attributes);
}

void DrvFtaeAlarm::SQLServerDatabaseEngine::loadServerInstances(std::string driverName)
{
	connection->ConnectToServerInstances(driverName);
}

void DrvFtaeAlarm::SQLServerDatabaseEngine::loadDatabaseInstances(std::string serverName, AuthenticationType type, std::string user, std::string password)
{
	connection->ConnectToDatabaseInstances(serverName, user, password);
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::ChooseDatabase(std::string databaseName)
{
	bool bConnected = connection->ConnectToDatabase(databaseName);
	
	return bConnected;
}

void DrvFtaeAlarm::SQLServerDatabaseEngine::CloseConnection()
{
	connection.reset();
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

std::vector<DrvFtaeAlarm::Record> DrvFtaeAlarm::SQLServerDatabaseEngine::ExecuteStatement(const std::vector<DrvFtaeAlarm::StatementCondition>& conditions)
{
	return std::vector<DrvFtaeAlarm::Record>{};
}