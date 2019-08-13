#pragma once
#include"DatabaseEngine.h"
#include"SQLServerConnection.h"
#include"SQLServerStatement.h"
#include"SQLDatabase.h"

namespace DrvFtaeAlarm {
	class SQLServerDatabaseEngine final: public DatabaseEngine
	{
	public:
		SQLServerDatabaseEngine() = default;
		SQLServerDatabaseEngine(const SQLServerDatabaseEngine& src) = delete;
		SQLServerDatabaseEngine(SQLServerDatabaseEngine&& src) = delete;
		SQLServerDatabaseEngine& operator=(const SQLServerDatabaseEngine& rhs) = delete;
		SQLServerDatabaseEngine& operator=(SQLServerDatabaseEngine&& rhs) = delete;
		bool OpenConnection() override;
		bool OpenConnection(const ConnectionAttributes& attributes) override;
		bool OpenConnectionIfNeeded(const ConnectionAttributes& attributes) override;
		bool loadServerInstances(std::string driverName) override;
		bool loadDatabaseInstances(std::string serverName, AuthenticationType type, std::string user, std::string password) override;
		bool ChooseDatabase(std::string databaseName) override;
		void CloseConnection() override;
		bool IsValidConnection() const override;
		ConnectionAttributes GetConnectionAttributes() const override;
		ConnectionStatus GetConnectionStatus() const override;
		std::vector<std::string> GetServersList() const override;
		std::vector<std::string> GetDatabasesList() const override;
		std::vector<Record> ExecuteStatement(const std::string& query, const std::vector<std::string>& parameters) override;
		~SQLServerDatabaseEngine();
	private:
		std::shared_ptr<SQLServerEnvironment> environment;
		std::shared_ptr<SQLServerConnection> connection;
	};
}