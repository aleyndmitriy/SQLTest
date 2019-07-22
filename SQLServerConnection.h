#pragma once
#include<Windows.h>
#include <sql.h>
#include <sqlext.h>
#include"Connection.h"
#include"SQLServerEnvironment.h"
#include"ConnectionAttributes.h"

namespace DrvFtaeAlarm {
	class SQLServerConnection final : public Connection {
	public:
		SQLServerConnection() = delete;
		SQLServerConnection(SQLServerConnection&& rsc) = delete;
		SQLServerConnection& operator=(SQLServerConnection&& rhs) = delete;
		explicit SQLServerConnection(const std::shared_ptr<SQLServerEnvironment>& environment, const ConnectionAttributes& attributes);
		explicit SQLServerConnection(std::shared_ptr<SQLServerEnvironment>&& environment, ConnectionAttributes&& attributes);
		SQLServerConnection(const SQLServerConnection& src) = default;
		SQLServerConnection& operator=(const SQLServerConnection& rhs) = default;
		~SQLServerConnection() override;
		void* GetInterface() override;
	private:
		SQLHDBC sqlDBC;
		void freeConnection();
		void allocateConnection();
	};
}