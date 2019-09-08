#include"SQLServerDatabaseInfoDAO.h"
#include"Log.h"

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
	std::string sql = std::string("SELECT TABLE_NAME, TABLE_SCHEMA FROM ") + databaseName +  std::string(".INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = '?'");
	std::vector<std::string> vec = { "BASE TABLE" };
	std::vector<Record> records = _databaseEngine->ExecuteStatement(sql, vec);
	if (records.empty()) {
		return nullptr;
	}
	std::unique_ptr<SQLDatabase> ptrData = std::make_unique<SQLDatabase>(databaseName);
	for (std::vector<Record>::const_iterator itr = records.cbegin(); itr != records.cend(); ++itr) {
		Record::const_iterator recordItrName = itr->findColumnValue("TABLE_NAME");
		Record::const_iterator recordItrSchema = itr->findColumnValue("TABLE_SCHEMA");
		if (recordItrName != itr->cend() && recordItrSchema != itr->cend()) {
			std::string schema = recordItrSchema->second.second;
			std::string name = recordItrName->second.second;
			ptrData->InsertTable(SQLTable(name,schema));
		}
	}
	return ptrData;
}

std::unique_ptr<DrvFtaeAlarm::SQLTable> DrvFtaeAlarm::SQLServerDatabaseInfoDAO::GetTableInfo(bool isManageConnection, const ConnectionAttributes& attributes, std::string databaseName, std::string tableName)
{
	if (isManageConnection) {
		_databaseEngine->CloseConnection();
	}
	if (!_databaseEngine->OpenConnectionIfNeeded(attributes)) {
		Log::GetInstance()->WriteInfo(_T("Can't connect to database"));
		return nullptr;
	}
	std::string querry = std::string("SELECT COLUMN_NAME, DATA_TYPE, TABLE_SCHEMA FROM Information_schema.Columns WHERE TABLE_NAME = '") + tableName + std::string("';");
	std::vector<std::string> vec = { };
	std::vector<Record> records = _databaseEngine->ExecuteStatement(querry, vec);
	if (isManageConnection) {
		_databaseEngine->CloseConnection();
	}
	if (records.empty()) {
		return nullptr;
	}
	std::unique_ptr<SQLTable> ptrData = std::make_unique<SQLTable>(tableName);
	for (std::vector<Record>::const_iterator itr = records.cbegin(); itr != records.cend(); ++itr) {
		Record::const_iterator recordItrColName = itr->findColumnValue("COLUMN_NAME");
		Record::const_iterator recordItrColType = itr->findColumnValue("DATA_TYPE");
		Record::const_iterator recordItrColSchema = itr->findColumnValue("TABLE_SCHEMA");
		if (recordItrColName != itr->cend() && recordItrColType != itr->cend() && recordItrColSchema != itr->cend()) {
			ptrData->InsertColumn(recordItrColName->second.second, recordItrColType->second.second);
			ptrData->SetSchemaName(recordItrColSchema->second.second);
		}
	}
	return ptrData;
}