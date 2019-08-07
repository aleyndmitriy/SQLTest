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
	if (attr.databaseName.empty() == false) {
		std::pair<std::map<std::string, SQLDatabase>::const_iterator, bool> res =
			databases.insert(std::make_pair<std::string, SQLDatabase>(std::string(attr.databaseName), SQLDatabase(attr.databaseName)));
		return (res.second || res.first != databases.cend());
	}
	return true;
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::loadServerInstances(std::string driverName)
{
	bool isConnect = connection->ConnectToServerInstances(driverName);
	return isConnect;
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::loadDatabaseInstances(std::string serverName, AuthenticationType type, std::string user, std::string password)
{
	bool isConnect = connection->ConnectToDatabaseInstances(serverName, user, password);
	return isConnect;
}

bool DrvFtaeAlarm::SQLServerDatabaseEngine::ChooseDatabase(std::string databaseName)
{
	if (connection->ConnectToDatabase(databaseName)) {
		std::pair<std::map<std::string, SQLDatabase>::const_iterator,bool> res = 
			databases.insert(std::make_pair<std::string, SQLDatabase>(std::string(databaseName), SQLDatabase(databaseName)));
		return (res.second || res.first != databases.cend());
	}
	return false;
}

void DrvFtaeAlarm::SQLServerDatabaseEngine::CloseConnection()
{
	databases.clear();
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

std::vector<DrvFtaeAlarm::Record> DrvFtaeAlarm::SQLServerDatabaseEngine::ExecuteStatement(const std::string& query, const std::vector<std::string>& parameters)
{
	
	
	return std::vector<DrvFtaeAlarm::Record>{};
}

