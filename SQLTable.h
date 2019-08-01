#pragma once
#include<map>
#include<string>

namespace DrvFtaeAlarm {
	
	class SQLTable
	{
	public:
		using const_iterator = std::map<std::string, std::string>::const_iterator;
		using value_type = std::pair<std::string, std::string>;
		using reference = value_type &;
		using const_reference = const value_type &;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		SQLTable(std::string tableName);
		SQLTable(const SQLTable& src) = default;
		SQLTable(SQLTable&& src) noexcept = default;
		SQLTable& operator=(const SQLTable& src) = default;
		SQLTable& operator=(SQLTable&& src) = default;
		~SQLTable();
		std::string GetTableName() const;
		bool InsertColumn(const std::string& columnName, const std::string& columnType);
		std::string& at(std::string columnName);
		const_iterator cbegin() const;
		const_iterator cend() const;
	private:
		std::string _tableName;
		std::map<std::string, std::string> columns;
	};

	bool operator==(const SQLTable& lhs, const SQLTable& rhs);
	bool operator!=(const SQLTable& lhs, const SQLTable& rhs);
	bool operator<(const SQLTable& lhs, const SQLTable& rhs);
	bool operator<=(const SQLTable& lhs, const SQLTable& rhs);
	bool operator>(const SQLTable& lhs, const SQLTable& rhs);
	bool operator>=(const SQLTable& lhs, const SQLTable& rhs);

}