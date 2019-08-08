#pragma once
#include "DatabaseInfoDAO.h"
#include"SQLServerDatabaseEngine.h"

namespace DrvFtaeAlarm {

	class SQLServerDatabaseInfoDAO final: public DatabaseInfoDAO {
	public:
		SQLServerDatabaseInfoDAO() = delete;
		SQLServerDatabaseInfoDAO(const std::shared_ptr<SQLServerDatabaseEngine>& databaseEngine);
		SQLServerDatabaseInfoDAO(const SQLServerDatabaseInfoDAO& src) = default;
		SQLServerDatabaseInfoDAO& operator=(const SQLServerDatabaseInfoDAO& src) = default;
		SQLServerDatabaseInfoDAO(SQLServerDatabaseInfoDAO&& rhs) noexcept = default;
		SQLServerDatabaseInfoDAO& operator=(SQLServerDatabaseInfoDAO&& rhs) = default;
		~SQLServerDatabaseInfoDAO();
		std::unique_ptr<SQLDatabase> GetDatabaseInfo(const ConnectionAttributes& attributes, std::string databaseName) override;
		std::unique_ptr<SQLTable> GetTableInfo(const ConnectionAttributes& attributes, std::string databaseName, std::string tableName) override;
	private:
		std::shared_ptr<SQLServerDatabaseEngine> _databaseEngine;
	};
}
