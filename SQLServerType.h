#pragma once
#include <string>
#include<utility>
#include<Windows.h>
#include<sqlext.h>
namespace DrvFtaeAlarm {

	enum class SQLServerType
	{
		UNKNOWN = -1,
		UNIQUEIDENTIFIER = 0,
		NVARCHAR = 1,
		NTEXT = 2,
		VARCHAR = 3,
		TEXT = 4,
		NCHAR = 5,
		CHAR = 6,
		DATE = 7,
		TIME = 8,
		DATETIME = 9,
		DATETIME2 = 10,
		SMALLDATETIME = 11,
		DATETIMEOFFSET = 12,
		BINARY = 13,
		VARBINARY = 14,
		NUMERIC = 15,
		INT = 16,
		SMALLINT = 17,
		TINYINT = 18,
		BIGINT = 19,
		BIT = 20,
		REAL = 21,
		FLOAT = 22,
		XML = 23
	};

	SQLServerType SQLServerTypesFromString(const std::string& type);
	std::pair<SQLSMALLINT, SQLSMALLINT> GetSQLServerTypeIdentifier(SQLServerType type);
}
