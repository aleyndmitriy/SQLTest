#include"SQLServerConditionRecordsDAO.h"

DrvFtaeAlarm::SQLServerConditionRecordsDAO::SQLServerConditionRecordsDAO(const std::shared_ptr<SQLServerDatabaseEngine>& databaseEngine): ConditionRecordsDAO(databaseEngine)
{

}

DrvFtaeAlarm::SQLServerConditionRecordsDAO::~SQLServerConditionRecordsDAO()
{
	_databaseEngine.reset();
}

std::vector<DrvFtaeAlarm::Record> DrvFtaeAlarm::SQLServerConditionRecordsDAO::GetRecords(const SQLTable& table, const ConnectionAttributes& attributes, const std::vector<StatementCondition>& conditions)
{
	std::vector<Record> records;
	if (!_databaseEngine->OpenConnectionIfNeeded(attributes)) {
		return records;
	}
	std::string querry = ConvertStatementsConditionToSQL(table,conditions);
	HANDLE hFile = CreateFile("SQLQuerry.sql", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return records;
	}
	BOOL isWrite = FALSE;
	DWORD posPointer = 0;
	size_t sz;
	isWrite = WriteFile(hFile, querry.c_str(), querry.size() + 1, &posPointer, NULL);
	if (posPointer != querry.size() + 1) {
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
		return records;
	}
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;
	std::vector<std::string> params = { };
	records = _databaseEngine->ExecuteStatement(querry, params);
	return records;
}

std::string DrvFtaeAlarm::SQLServerConditionRecordsDAO::ConvertStatementsConditionToSQL(const SQLTable& table, const std::vector<StatementCondition>& conditions)
{
	std::string sql = std::string("SELECT ");
	for (SQLTable::const_iterator itr = table.cbegin(); itr != table.cend(); ++itr) {
		sql += std::string(" ") + table.GetFullName() + std::string(".") + itr->first + std::string(", ");
	}
	sql.erase(sql.size() - 2, 2);
	sql += std::string(" FROM ") + table.GetFullName() + std::string(" WHERE ");
	std::vector<StatementCondition>::const_iterator itr = conditions.cbegin();
	sql += ConvertStatementConditionToSQL(table, *itr);
	++itr;
	while(itr != conditions.cend()) {
		switch (itr->GetCombineOperation()) {
		case CombineOperation::COMBINEOP_AND:
			sql += std::string(" AND ");
			break;
		case CombineOperation::COMBINEOP_OR:
			sql += std::string(" OR ");
			break;
		}
		sql += ConvertStatementConditionToSQL(table, *itr);
		++itr;
	}
	return sql;
}

std::string DrvFtaeAlarm::SQLServerConditionRecordsDAO::ConvertStatementConditionToSQL(const SQLTable& table, const StatementCondition& condition)
{
	std::string sql = table.GetFullName() + std::string(".") + condition.GetProperty();
	std::string val1 = condition.GetValue1();
	std::string val2 = condition.GetValue2();
	switch (condition.GetPropertyType()) {
	case PropertyType::PROPTYPE_TEXT:
	case PropertyType::PROPTYPE_DATE:
		if (!val1.empty()) {
			val1 = std::string("'") + val1 + std::string("'");
		}
		if (!val2.empty()) {
			val2 = std::string("'") + val2 + std::string("'");
		}
		break;
	default:
		break;
	}
	switch (condition.GetConditionType()) {
	case ConditionType::CONDTYPE_EQUAL:
		sql += std::string(" = ") + val1;
		break;
	case ConditionType::CONDTYPE_LESS:
		sql += std::string(" < ") + val1;
		break;
	case ConditionType::CONDTYPE_GREATER:
		sql += std::string(" > ") + val1;
		break;
	case ConditionType::CONDTYPE_LESSEQUAL:
		sql += std::string(" <= ") + val1;
		break;
	case ConditionType::CONDTYPE_GREATEREQUAL:
		sql += std::string(" >= ") + val1;
		break;
	case ConditionType::CONDTYPE_NOTEQUAL:
		sql += std::string(" != ") + val1;
		break;
	case ConditionType::CONDTYPE_BETWEEN:
		sql += std::string(" BETWEEN ") + val1 + std::string(" AND ") + val2;
		break;
	case ConditionType::CONDTYPE_ISNULL:
		sql += std::string(" IS NULL ");
		break;
	case ConditionType::CONDTYPE_ISNOTNULL:
		sql += std::string(" IS NOT NULL ");
		break;
	case ConditionType::CONDTYPE_LIKE:
		sql += std::string(" LIKE ") + std::string(" '") + condition.GetValue1() + std::string("%' ");
		break;
	default:
		break;
	}
	return sql;
}