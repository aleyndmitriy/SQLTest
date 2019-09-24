#pragma once
#include"SQLDatabase.h"
#include "DatabaseEngine.h"
#include<memory>

namespace DrvFtaeAlarm {

	class DatabaseInfoDAO {
	public:
		DatabaseInfoDAO() = delete;
		DatabaseInfoDAO(const DatabaseInfoDAO& src) = default;
		DatabaseInfoDAO& operator=(const DatabaseInfoDAO& src) = default;
		DatabaseInfoDAO(DatabaseInfoDAO&& rhs) noexcept = default;
		DatabaseInfoDAO& operator=(DatabaseInfoDAO&& rhs) = default;
		DatabaseInfoDAO(const std::shared_ptr<DatabaseEngine>& databaseEngine);
		~DatabaseInfoDAO();
		virtual std::unique_ptr<SQLDatabase> GetDatabaseInfo(const ConnectionAttributes& attributes, std::string databaseName) = 0;
		virtual std::unique_ptr<SQLTable> GetTableInfo(bool isManageConnection, const ConnectionAttributes& attributes, std::string databaseName, std::string tableName) = 0;
		virtual std::unique_ptr<SQLTable> GetTableInfoInDirectConnection(const ConnectionAttributes& attributes, std::string databaseName, std::string tableName) = 0;
	protected:
		std::shared_ptr<DatabaseEngine> _databaseEngine;
	};

}
