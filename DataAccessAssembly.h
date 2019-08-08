#pragma once
#include<memory>
#include"DatabaseEngine.h"
#include"ISettingsDataSource.h"
#include "DatabaseInfoDAO.h"

namespace DrvFtaeAlarm {

	class DataAccessAssembly {
	public:
		virtual std::shared_ptr<DatabaseEngine> GetDataBaseEngine() const = 0;
		virtual std::shared_ptr<ISettingsDataSource> GetSettingDataSource() const = 0;
		virtual std::shared_ptr<DatabaseInfoDAO> GetDatabaseInfoSQLServerDao() const = 0;
		DataAccessAssembly(const DataAccessAssembly& src) = delete;
		DataAccessAssembly& operator=(const DataAccessAssembly& src) = delete;
		DataAccessAssembly(DataAccessAssembly&& rhs) = delete;
		DataAccessAssembly& operator=(DataAccessAssembly&& rhs) = delete;
	protected:
		DataAccessAssembly() = default;
		virtual ~DataAccessAssembly() { };
	};
}