#pragma once
#include<Windows.h>
#include <sql.h>
#include <sqlext.h>
#include"Connection.h"
#include"SQLServerEnvironment.h"
#include"ConnectionAttributes.h"
#include<vector>

namespace DrvFtaeAlarm {
	class SQLServerDatabaseEngine;
	class SQLServerStatement;

	class SQLServerConnection final : public Connection {
	public:
		friend class SQLServerDatabaseEngine;
		friend class SQLServerStatement;
		friend class std::_Ref_count_obj<SQLServerConnection>;
	private:
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
		void* GetInterface(int nIfcId) override;
		bool ConnectToServerInstances(std::string driverName) override;
		bool ConnectToDatabaseInstances(std::string serverName, std::string login, std::string password, AuthenticationType authType) override;
		bool ConnectToDatabase(std::string databaseName) override;
		std::vector<std::string> GetServerList() const override;
		std::vector<std::string> GetDatabaseList() const override;
		bool IsValidConnection() const override;
		ConnectionStatus GetConnectionStatus() const override;

		SQLHDBC sqlDBC;
		void freeConnection();
		void allocateConnection();
		void HandleDiagnosticRecord();
		std::vector<std::string> serverList;
		std::vector<std::string> databaseList;
	};
}