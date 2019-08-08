#pragma once
#include"DataAccessAssembly.h"
#include"SQLServerDatabaseEngine.h"
#include "XMLSettingsDataSource.h"
#include "SQLServerDatabaseInfoDAO.h"

namespace DrvFtaeAlarm {

	class DataSQLServerAccessAssembly final : public DataAccessAssembly {
	public:
		std::shared_ptr<DatabaseEngine> GetDataBaseEngine() const override;
		std::shared_ptr<ISettingsDataSource> GetSettingDataSource() const override;
		std::shared_ptr<DatabaseInfoDAO> GetDatabaseInfoSQLServerDao() const override;
		DataSQLServerAccessAssembly(const DataSQLServerAccessAssembly& src) = delete;
		DataSQLServerAccessAssembly& operator=(const DataSQLServerAccessAssembly& src) = delete;
		DataSQLServerAccessAssembly(DataSQLServerAccessAssembly&& rhs) = delete;
		DataSQLServerAccessAssembly& operator=(DataSQLServerAccessAssembly&& rhs) = delete;
		static DataSQLServerAccessAssembly& instance();
	private:
		DataSQLServerAccessAssembly();
		~DataSQLServerAccessAssembly();
		std::shared_ptr<SQLServerDatabaseEngine> databaseEngine;
		std::shared_ptr<XMLSettingsDataSource> settingsDataSource;
		std::shared_ptr<SQLServerDatabaseInfoDAO> databaseInfoDAO;
	};
}