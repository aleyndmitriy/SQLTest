#include"SQLDatabase.h"

DrvFtaeAlarm::SQLDatabase::SQLDatabase(std::string databaseName) :_databaseName(databaseName), tables{}
{
	
}

DrvFtaeAlarm::SQLDatabase::~SQLDatabase()
{
	_databaseName.clear();
	tables.clear();
}

std::string DrvFtaeAlarm::SQLDatabase::GetDatabaseName() const
{
	return _databaseName;
}

DrvFtaeAlarm::SQLDatabase::const_iterator DrvFtaeAlarm::SQLDatabase::cbegin() const
{
	return tables.cbegin();
}

DrvFtaeAlarm::SQLDatabase::const_iterator DrvFtaeAlarm::SQLDatabase::cend() const
{
	return tables.cend();
}

DrvFtaeAlarm::SQLTable& DrvFtaeAlarm::SQLDatabase::at(std::string tableName)
{
	return tables.at(tableName);
}


bool DrvFtaeAlarm::SQLDatabase::InsertTable(SQLTable&& table)
{
	std::pair<std::string, SQLTable> val = std::make_pair<std::string, SQLTable>(std::string(table.GetTableName()), std::move(table));
	std::pair<std::map<std::string, SQLTable>::iterator, bool> res = tables.insert(val);
	return res.second;
}

bool DrvFtaeAlarm::operator==(const DrvFtaeAlarm::SQLDatabase& lhs, const DrvFtaeAlarm::SQLDatabase& rhs)
{
	return lhs.GetDatabaseName() == rhs.GetDatabaseName();
}

bool DrvFtaeAlarm::operator!=(const DrvFtaeAlarm::SQLDatabase& lhs, const DrvFtaeAlarm::SQLDatabase& rhs)
{
	return lhs.GetDatabaseName() != rhs.GetDatabaseName();
}

bool DrvFtaeAlarm::operator<(const DrvFtaeAlarm::SQLDatabase& lhs, const DrvFtaeAlarm::SQLDatabase& rhs)
{
	return lhs.GetDatabaseName() < rhs.GetDatabaseName();
}

bool DrvFtaeAlarm::operator<=(const DrvFtaeAlarm::SQLDatabase& lhs, const DrvFtaeAlarm::SQLDatabase& rhs)
{
	return lhs.GetDatabaseName() <= rhs.GetDatabaseName();
}

bool DrvFtaeAlarm::operator>(const DrvFtaeAlarm::SQLDatabase& lhs, const DrvFtaeAlarm::SQLDatabase& rhs)
{
	return lhs.GetDatabaseName() > rhs.GetDatabaseName();
}

bool DrvFtaeAlarm::operator>=(const DrvFtaeAlarm::SQLDatabase& lhs, const DrvFtaeAlarm::SQLDatabase& rhs)
{
	return lhs.GetDatabaseName() >= rhs.GetDatabaseName();
}