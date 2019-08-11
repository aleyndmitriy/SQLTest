#include"SQLServerType.h"

DrvFtaeAlarm::SQLServerType DrvFtaeAlarm::SQLServerTypesFromString(const std::string& type)
{
	if (type == std::string("uniqueidentifier")) {
		return SQLServerType::UNIQUEIDENTIFIER;
	}
	else if (type == std::string("nvarchar")) {
		return SQLServerType::NVARCHAR;
	}
	else if (type == std::string("varchar")) {
		return SQLServerType::VARCHAR;
	}
	else if (type == std::string("ntext")) {
		return SQLServerType::NTEXT;
	}
	else if (type == std::string("text")) {
		return SQLServerType::TEXT;
	}
	else if (type == std::string("nchar")) {
		return SQLServerType::NCHAR;
	}
	else if (type == std::string("char")) {
		return SQLServerType::CHAR;
	}
	else if (type == std::string("date")) {
		return SQLServerType::DATE;
	}
	else if (type == std::string("time")) {
		return SQLServerType::TIME;
	}
	else if (type == std::string("datetime")) {
		return SQLServerType::DATETIME;
	}
	else if (type == std::string("datetime2")) {
		return SQLServerType::DATETIME2;
	}
	else if (type == std::string("smalldatetime")) {
		return SQLServerType::SMALLDATETIME;
	}
	else if (type == std::string("datetimeoffset")) {
		return SQLServerType::DATETIMEOFFSET;
	}
	else if (type == std::string("binary")) {
		return SQLServerType::BINARY;
	}
	else if (type == std::string("varbinary")) {
		return SQLServerType::VARBINARY;
	}
	else if (type == std::string("numeric")) {
		return SQLServerType::NUMERIC;
	}
	else if (type == std::string("int")) {
		return SQLServerType::INT;
	}
	else if (type == std::string("smallint")) {
		return SQLServerType::SMALLINT;
	}
	else if (type == std::string("tinyint")) {
		return SQLServerType::TINYINT;
	}
	else if (type == std::string("bigint")) {
		return SQLServerType::BIGINT;
	}
	else if (type == std::string("bit")) {
		return SQLServerType::BIT;
	}
	else if (type == std::string("real")) {
		return SQLServerType::REAL;
	}
	else if (type == std::string("float")) {
		return SQLServerType::FLOAT;
	}
	else if (type == std::string("xml")) {
		return SQLServerType::XML;
	}
	else {
		return SQLServerType::UNKNOWN;
	}
}

SQLSMALLINT  DrvFtaeAlarm::GetSQLServerTypeIdentifier(SQLServerType type)
{
	switch (type)
	{
	case SQLServerType::UNIQUEIDENTIFIER:
		return SQL_C_GUID;
		break;
	case SQLServerType::NVARCHAR:
	case SQLServerType::VARCHAR:
	case SQLServerType::NTEXT:
	case SQLServerType::TEXT:
	case SQLServerType::NCHAR:
	case SQLServerType::CHAR:
	case SQLServerType::XML:
	case SQLServerType::NUMERIC:
		return SQL_C_CHAR;
		break;
	case SQLServerType::DATE:
		return SQL_C_TYPE_DATE;
		break;
	case SQLServerType::TIME:
		return SQL_C_TYPE_TIME;
		break;
	case SQLServerType::DATETIME:
	case SQLServerType::DATETIME2:
	case SQLServerType::SMALLDATETIME:
		return SQL_C_TYPE_TIMESTAMP;
		break;
	case SQLServerType::DATETIMEOFFSET:
		return SQL_C_INTERVAL_SECOND;
		break;
	case SQLServerType::BINARY:
	case SQLServerType::VARBINARY:
		return SQL_C_BINARY;
		break;
	case SQLServerType::INT:
		return SQL_C_LONG;
		break;
	case SQLServerType::SMALLINT:
		return SQL_C_SHORT;
		break;
	case SQLServerType::TINYINT:
		return SQL_C_TINYINT;
		break;
	case SQLServerType::BIGINT:
		return SQL_C_UBIGINT;
		break;
	case SQLServerType::BIT:
		return SQL_C_BIT;
		break;
	case SQLServerType::REAL:
		return SQL_C_FLOAT;
		break;
	case SQLServerType::FLOAT:
		return SQL_FLOAT;
		break;
	default:
		return SQL_DEFAULT;
			break;
	}
}

DrvFtaeAlarm::PropertyType DrvFtaeAlarm::PropertyTypeFromString(const std::string& type)
{
	SQLSMALLINT identifier = GetSQLServerTypeIdentifier(SQLServerTypesFromString(type));
	switch (identifier)
	{
	case SQL_C_GUID:
	case SQL_C_CHAR:
	case SQL_C_BINARY:
		return PropertyType::PROPTYPE_TEXT;
		break;
	case SQL_C_TYPE_DATE:
	case SQL_C_TYPE_TIME:
	case SQL_C_TYPE_TIMESTAMP:
		return PropertyType::PROPTYPE_DATE;
		break;
	case SQL_C_LONG:
	case SQL_C_SHORT:
	case SQL_C_TINYINT:
	case SQL_C_UBIGINT:
	case SQL_C_FLOAT:
	case SQL_FLOAT:
	case SQL_C_INTERVAL_SECOND:
		return PropertyType::PROPTYPE_NUMERIC;
		break;
	case SQL_C_BIT:
		return PropertyType::PROPTYPE_BOOLEAN;
		break;
	default:
		return PropertyType::PROPTYPE_NONE;
		break;
	}
}
