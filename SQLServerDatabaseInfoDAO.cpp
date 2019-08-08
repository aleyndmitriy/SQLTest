#include"SQLServerDatabaseInfoDAO.h"

DrvFtaeAlarm::SQLServerDatabaseInfoDAO::SQLServerDatabaseInfoDAO(const std::shared_ptr<SQLServerDatabaseEngine>& databaseEngine):DatabaseInfoDAO(databaseEngine)
{

}

DrvFtaeAlarm::SQLServerDatabaseInfoDAO::~SQLServerDatabaseInfoDAO()
{
	_databaseEngine.reset();
}

std::unique_ptr<DrvFtaeAlarm::SQLDatabase> DrvFtaeAlarm::SQLServerDatabaseInfoDAO::GetDatabaseInfo(const ConnectionAttributes& attributes, std::string databaseName)
{
	if (!_databaseEngine->OpenConnectionIfNeeded(attributes)) {
		return nullptr;
	}
	std::string sql = std::string("SELECT TABLE_NAME FROM ") + databaseName +  std::string(".INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = '?'");
	std::vector<std::string> vec = { "BASE TABLE" };
	std::vector<Record> records = _databaseEngine->ExecuteStatement(sql, vec);
	if (records.empty()) {
		return nullptr;
	}
	std::unique_ptr<SQLDatabase> ptrData = std::make_unique<SQLDatabase>(databaseName);
	for (std::vector<Record>::const_iterator itr = records.cbegin(); itr != records.cend(); ++itr) {
		Record::const_iterator recordItr = itr->findColumnValue("TABLE_NAME");
		if (recordItr != itr->cend()) {
			ptrData->InsertTable(SQLTable(recordItr->second.second));
		}
	}
	return ptrData;
}

std::unique_ptr<DrvFtaeAlarm::SQLTable> DrvFtaeAlarm::SQLServerDatabaseInfoDAO::GetTableInfo(const ConnectionAttributes& attributes, std::string databaseName, std::string tableName)
{
	if (!_databaseEngine->OpenConnectionIfNeeded(attributes)) {
		return nullptr;
	}
	std::string querry = std::string("SELECT COLUMN_NAME, DATA_TYPE FROM Information_schema.Columns WHERE TABLE_NAME = '") + tableName + std::string("';");
	std::vector<std::string> vec = { };
	std::vector<Record> records = _databaseEngine->ExecuteStatement(querry, vec);
	if (records.empty()) {
		return nullptr;
	}
	std::unique_ptr<SQLTable> ptrData = std::make_unique<SQLTable>(tableName);
	for (std::vector<Record>::const_iterator itr = records.cbegin(); itr != records.cend(); ++itr) {
		Record::const_iterator recordItrColName = itr->findColumnValue("COLUMN_NAME");
		Record::const_iterator recordItrColType = itr->findColumnValue("DATA_TYPE");
		if (recordItrColName != itr->cend() && recordItrColType != itr->cend()) {
			ptrData->InsertColumn(recordItrColName->second.second, recordItrColType->second.second);
		}
	}
	return ptrData;
}