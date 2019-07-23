#pragma once
#include<Windows.h>
#include <sql.h>
#include <sqlext.h>
#include"Connection.h"
#include"SQLServerEnvironment.h"
#include"ConnectionAttributes.h"
#include<vector>
namespace DrvFtaeAlarm {
	class SQLServerConnection final : public Connection {
	public:
		SQLServerConnection() = delete;
		SQLServerConnection(SQLServerConnection&& rsc) = delete;
		SQLServerConnection& operator=(SQLServerConnection&& rhs) = delete;
		explicit SQLServerConnection(const std::shared_ptr<SQLServerEnvironment>& environment, const ConnectionAttributes& attributes);
		explicit SQLServerConnection(const std::shared_ptr<SQLServerEnvironment>& environment);
		explicit SQLServerConnection(std::shared_ptr<SQLServerEnvironment>&& environment, ConnectionAttributes&& attributes);
		explicit SQLServerConnection(std::shared_ptr<SQLServerEnvironment>&& environment);
		SQLServerConnection(const SQLServerConnection& src) = default;
		SQLServerConnection& operator=(const SQLServerConnection& rhs) = default;
		~SQLServerConnection() override;
		void* GetInterface() override;
		void ConnectToServerInstances(std::string driverName) override;
		void ConnectToDatabaseInstances(std::string serverName, std::string login, std::string password) override;
		void ConnectToDatabase(std::string databaseName) override;
		std::vector<std::string> GetServerList() const override;
		std::vector<std::string> GetDatabaseList() const override;
	private:
		SQLHDBC sqlDBC;
		void freeConnection();
		void allocateConnection();
		std::vector<std::string> serverList;
		std::vector<std::string> databaseList;
	};
}