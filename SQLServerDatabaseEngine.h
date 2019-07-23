#pragma once
#include"DatabaseEngine.h"
#include"SQLServerConnection.h"

namespace DrvFtaeAlarm {
	class SQLServerDatabaseEngine final: public DatabaseEngine
	{
	public:
		SQLServerDatabaseEngine() = default;
		SQLServerDatabaseEngine(const SQLServerDatabaseEngine& src) = delete;
		SQLServerDatabaseEngine(SQLServerDatabaseEngine&& src) = delete;
		SQLServerDatabaseEngine& operator=(const SQLServerDatabaseEngine& rhs) = delete;
		SQLServerDatabaseEngine& operator=(SQLServerDatabaseEngine&& rhs) = delete;
		void OpenConnection() override;
		void OpenConnection(const ConnectionAttributes& attributes) override;
		void loadServerInstances(std::string driverName) override;
		void loadDatabaseInstances(std::string serverName, AuthenticationType type, std::string user, std::string password) override;
		void ChooseDatabase(std::string databaseName) override;
		void CloseConnection() override;
		std::vector<std::string> GetServersList() const override;
		std::vector<std::string> GetDatabasesList() const override;
		 ~SQLServerDatabaseEngine();
	private:
		std::shared_ptr<SQLServerEnvironment> environment;
		std::shared_ptr<SQLServerConnection> connection;
	};
}