#pragma once
#include"SQLTable.h"

namespace DrvFtaeAlarm {

	class SQLDatabase
	{
	public:
		using const_iterator = std::map<std::string, SQLTable>::const_iterator;
		using value_type = std::pair<std::string, SQLTable>;
		using reference = value_type &;
		using const_reference = const value_type &;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		SQLDatabase(std::string databaseName);
		SQLDatabase(const SQLDatabase& database) = default;
		SQLDatabase& operator=(const SQLDatabase& database) = default;
		SQLDatabase(SQLDatabase&& database) = default;
		SQLDatabase& operator=(SQLDatabase&& database) = default;
		~SQLDatabase();
		std::string GetDatabaseName() const;
		const_iterator cbegin() const;
		const_iterator cend() const;
		SQLTable& at(std::string tableName);
		bool InsertTable(SQLTable&& table);
	private:
		std::string _databaseName;
		std::map<std::string, SQLTable> tables;
	};

	bool operator==(const SQLDatabase& lhs, const SQLDatabase& rhs);
	bool operator!=(const SQLDatabase& lhs, const SQLDatabase& rhs);
	bool operator<(const SQLDatabase& lhs, const SQLDatabase& rhs);
	bool operator<=(const SQLDatabase& lhs, const SQLDatabase& rhs);
	bool operator>(const SQLDatabase& lhs, const SQLDatabase& rhs);
	bool operator>=(const SQLDatabase& lhs, const SQLDatabase& rhs);

}