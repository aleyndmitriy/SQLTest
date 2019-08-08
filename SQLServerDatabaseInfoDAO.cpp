#include"SQLServerDatabaseInfoDAO.h"

DrvFtaeAlarm::SQLServerDatabaseInfoDAO::SQLServerDatabaseInfoDAO(const std::shared_ptr<SQLServerDatabaseEngine >& databaseEngine):DatabaseInfoDAO(databaseEngine)
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
	std::string sql = std::string("SELECT TABLE_NAME FROM ") + databaseName +  std::string(".INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = ?");
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
	std::string querry = std::string("SELECT sys.columns.name AS 'ColumnName', sys.types.Name 'DataType', sys.columns.max_length 'Max Length', sys.columns.precision, ISNULL(sys.indexes.is_primary_key, 0) 'Primary Key' FROM sys.columns") +
		std::string("INNER JOIN sys.types ON sys.columns.user_type_id = sys.types.user_type_id LEFT OUTER JOIN sys.index_columns ON sys.index_columns.object_id = sys.columns.object_id AND sys.index_columns.column_id = sys.columns.column_id") +
		std::string("LEFT OUTER JOIN sys.indexes ON sys.index_columns.object_id = sys.indexes.object_id AND sys.index_columns.index_id = sys.indexes.index_id WHERE") +
		std::string("sys.columns.object_id = OBJECT_ID('") + tableName + std::string("');");
	std::vector<std::string> vec = { };
	std::vector<Record> records = _databaseEngine->ExecuteStatement(querry, vec);
	if (records.empty()) {
		return nullptr;
	}
	std::unique_ptr<SQLTable> ptrData = std::make_unique<SQLTable>(tableName);
	for (std::vector<Record>::const_iterator itr = records.cbegin(); itr != records.cend(); ++itr) {
		Record::const_iterator recordItrColName = itr->findColumnValue("ColumnName");
		Record::const_iterator recordItrColType = itr->findColumnValue("DataType");
		if (recordItrColName != itr->cend() && recordItrColType != itr->cend()) {
			ptrData->InsertColumn(recordItrColName->second.second, recordItrColType->second.second);
		}
	}
	return ptrData;
}