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


DrvFtaeAlarm::DataSQLServerAccessAssembly::DataSQLServerAccessAssembly() :DataAccessAssembly(), databaseEngine(std::make_shared<DrvFtaeAlarm::SQLServerDatabaseEngine>()),settingsDataSource(std::make_shared<DrvFtaeAlarm::XMLSettingsDataSource>())
{

}

DrvFtaeAlarm::DataSQLServerAccessAssembly::~DataSQLServerAccessAssembly()
{
	databaseEngine.reset();
	settingsDataSource.reset();
}

DrvFtaeAlarm::DataSQLServerAccessAssembly& DrvFtaeAlarm::DataSQLServerAccessAssembly::instance()
{
	static DataSQLServerAccessAssembly shared;
	return shared;
}