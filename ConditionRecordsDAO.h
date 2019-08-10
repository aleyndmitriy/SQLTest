#pragma once
#include"Record.h"
#include "DatabaseEngine.h"
#include "StatementCondition.h"
#include "SQLTable.h"
#include<memory>

namespace DrvFtaeAlarm {

	class ConditionRecordsDAO {
	public:
		ConditionRecordsDAO() = delete;
		ConditionRecordsDAO(const std::shared_ptr<DatabaseEngine>& databaseEngine);
		ConditionRecordsDAO(const ConditionRecordsDAO& src) = default;
		ConditionRecordsDAO(ConditionRecordsDAO&& rhs) noexcept = default;
		ConditionRecordsDAO& operator=(const ConditionRecordsDAO& src) = default;
		ConditionRecordsDAO& operator=(ConditionRecordsDAO&& rhs) = default;
		~ConditionRecordsDAO();
		virtual std::vector<Record> GetRecords(const SQLTable& table, const ConnectionAttributes& attributes, const std::vector<StatementCondition>& conditions) = 0;
	protected:
		std::shared_ptr<DatabaseEngine> _databaseEngine;
	};
}