#include"DataSQLServerAccessAssembly.h"
#include"SQLServerDatabaseEngine.h"

std::shared_ptr<DrvFtaeAlarm::DatabaseEngine> DrvFtaeAlarm::DataSQLServerAccessAssembly::GetDataBaseEngine() const
{
	return databaseEngine;
}

DrvFtaeAlarm::DataSQLServerAccessAssembly::DataSQLServerAccessAssembly() :DataAccessAssembly(), databaseEngine(std::make_shared<DrvFtaeAlarm::SQLServerDatabaseEngine>())
{

}

DrvFtaeAlarm::DataSQLServerAccessAssembly::~DataSQLServerAccessAssembly()
{
	databaseEngine.reset();
}

DrvFtaeAlarm::DataSQLServerAccessAssembly& DrvFtaeAlarm::DataSQLServerAccessAssembly::instance()
{
	static DataSQLServerAccessAssembly shared;
	return shared;
}