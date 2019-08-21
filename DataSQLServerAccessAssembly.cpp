#include"DataSQLServerAccessAssembly.h"
#include"SQLServerDatabaseEngine.h"

std::shared_ptr<DrvFtaeAlarm::DatabaseEngine> DrvFtaeAlarm::DataSQLServerAccessAssembly::GetDataBaseEngine() const
{
	return databaseEngine;
}

std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> DrvFtaeAlarm::DataSQLServerAccessAssembly::GetSettingDataSource() const
{
	return settingsDataSource;
}


DrvFtaeAlarm::DataSQLServerAccessAssembly::DataSQLServerAccessAssembly(): DataAccessAssembly(), 
databaseEngine(std::make_shared<DrvFtaeAlarm::SQLServerDatabaseEngine>()),settingsDataSource(std::make_shared<DrvFtaeAlarm::XMLSettingsDataSource>()),
databaseInfoDAO(std::make_shared<SQLServerDatabaseInfoDAO>(databaseEngine)), conditionRecordsDAO(std::make_shared<SQLServerConditionRecordsDAO>(databaseEngine))
{
	
}

DrvFtaeAlarm::DataSQLServerAccessAssembly::~DataSQLServerAccessAssembly()
{
	conditionRecordsDAO.reset();
	databaseInfoDAO.reset();
	settingsDataSource.reset();
	databaseEngine.reset();
}

DrvFtaeAlarm::DataSQLServerAccessAssembly& DrvFtaeAlarm::DataSQLServerAccessAssembly::instance()
{
	static DataSQLServerAccessAssembly shared;
	return shared;
}

std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO> DrvFtaeAlarm::DataSQLServerAccessAssembly::GetDatabaseInfoSQLServerDao() const
{
	return databaseInfoDAO;
}

std::shared_ptr<DrvFtaeAlarm::ConditionRecordsDAO> DrvFtaeAlarm::DataSQLServerAccessAssembly::GetConditionRecordSQLServerDao() const {

	return conditionRecordsDAO;
}