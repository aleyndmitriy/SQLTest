#pragma once
#include"ConditionRecordsDAO.h"
#include"SQLServerDatabaseEngine.h"

namespace DrvFtaeAlarm {
	class SQLServerConditionRecordsDAO final : public ConditionRecordsDAO
	{
	public:
		SQLServerConditionRecordsDAO() = delete;
		SQLServerConditionRecordsDAO(const std::shared_ptr<SQLServerDatabaseEngine>& databaseEngine);
		SQLServerConditionRecordsDAO(const SQLServerConditionRecordsDAO& src) = default;
		SQLServerConditionRecordsDAO& operator=(const SQLServerConditionRecordsDAO& src) = default;
		SQLServerConditionRecordsDAO(SQLServerConditionRecordsDAO&& rhs) noexcept = default;
		SQLServerConditionRecordsDAO& operator=(SQLServerConditionRecordsDAO&& rhs) = default;
		~SQLServerConditionRecordsDAO();
		std::vector<Record> GetRecords(const SQLTable& table, const ConnectionAttributes& attributes, const std::vector<StatementCondition>& conditions) override;
	private:
		std::string ConvertStatementConditionToSQL(const SQLTable& table, const StatementCondition& condition);
		std::string ConvertStatementsConditionToSQL(const SQLTable& table, const std::vector<StatementCondition>& conditions);
	};
}