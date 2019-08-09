#include"SQLTable.h"


DrvFtaeAlarm::SQLTable::SQLTable(const std::string& tableName, const std::string& schemaName) :_tableName(tableName),_schemaName(schemaName), columns(std::map<std::string, std::string>{})
{

}

DrvFtaeAlarm::SQLTable::SQLTable(const std::string& tableName) : SQLTable(tableName,std::string())
{

}

DrvFtaeAlarm::SQLTable::~SQLTable()
{
	_tableName.clear();
	_schemaName.clear();
	columns.clear();
}

std::string DrvFtaeAlarm::SQLTable::GetTableName() const
{
	return _tableName;
}

std::string DrvFtaeAlarm::SQLTable::GetSchemaName() const
{
	return _schemaName;
}

std::string DrvFtaeAlarm::SQLTable::GetFullName() const
{
	if (_schemaName.empty()) {
		return _tableName;
	}
	else {
		return std::string(_schemaName + std::string(".") + _tableName);
	}
}

bool DrvFtaeAlarm::SQLTable::InsertColumn(const std::string& columnName, const std::string& columnType)
{
	std::pair<std::string, std::string> val = std::make_pair<std::string, std::string>(std::string(columnName), std::string(columnType));
	std::pair<std::map<std::string, std::string>::iterator, bool> res = columns.insert(val);
	return res.second;
}

DrvFtaeAlarm::SQLTable::const_iterator DrvFtaeAlarm::SQLTable::cbegin() const
{
	return columns.cbegin();
}

DrvFtaeAlarm::SQLTable::const_iterator DrvFtaeAlarm::SQLTable::cend() const
{
	return columns.cend();
}

std::string& DrvFtaeAlarm::SQLTable::at(std::string columnName)
{
	return columns.at(columnName);
}


bool DrvFtaeAlarm::operator==(const DrvFtaeAlarm::SQLTable& lhs, const DrvFtaeAlarm::SQLTable& rhs)
{
	return lhs.GetFullName() == rhs.GetFullName();
}

bool DrvFtaeAlarm::operator!=(const DrvFtaeAlarm::SQLTable& lhs, const DrvFtaeAlarm::SQLTable& rhs)
{
	return lhs.GetFullName() != rhs.GetFullName();
}

bool DrvFtaeAlarm::operator<(const DrvFtaeAlarm::SQLTable& lhs, const DrvFtaeAlarm::SQLTable& rhs)
{
	return lhs.GetFullName() < rhs.GetFullName();
}

bool DrvFtaeAlarm::operator<=(const DrvFtaeAlarm::SQLTable& lhs, const DrvFtaeAlarm::SQLTable& rhs)
{
	return lhs.GetFullName() <= rhs.GetFullName();
}

bool DrvFtaeAlarm::operator>(const DrvFtaeAlarm::SQLTable& lhs, const DrvFtaeAlarm::SQLTable& rhs)
{
	return lhs.GetFullName() > rhs.GetFullName();
}

bool DrvFtaeAlarm::operator>=(const DrvFtaeAlarm::SQLTable& lhs, const DrvFtaeAlarm::SQLTable& rhs)
{
	return lhs.GetFullName() >= rhs.GetFullName();
}