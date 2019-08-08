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
		return SQL_GUID;
		break;
	case SQLServerType::NVARCHAR:
	case SQLServerType::VARCHAR:
	case SQLServerType::NTEXT:
	case SQLServerType::TEXT:
	case SQLServerType::NCHAR:
	case SQLServerType::CHAR:
	case SQLServerType::XML:
	case SQLServerType::NUMERIC:
		return SQL_CHAR;
		break;
	case SQLServerType::DATE:
		return SQL_TYPE_DATE;
		break;
	case SQLServerType::TIME:
		return SQL_TYPE_TIME;
		break;
	case SQLServerType::DATETIME:
	case SQLServerType::DATETIME2:
	case SQLServerType::SMALLDATETIME:
		return SQL_TYPE_TIMESTAMP;
		break;
	case SQLServerType::DATETIMEOFFSET:
		return SQL_INTERVAL_SECOND;
		break;
	case SQLServerType::BINARY:
	case SQLServerType::VARBINARY:
		return SQL_BINARY;
		break;
	case SQLServerType::INT:
		return SQL_C_SLONG;
		break;
	case SQLServerType::SMALLINT:
		return SQL_C_SSHORT;
		break;
	case SQLServerType::TINYINT:
		return SQL_C_STINYINT;
		break;
	case SQLServerType::BIGINT:
		return SQL_C_SBIGINT;
		break;
	case SQLServerType::BIT:
		return SQL_BIT;
		break;
	case SQLServerType::REAL:
		return SQL_REAL;
		break;
	case SQLServerType::FLOAT:
		return SQL_FLOAT;
		break;
	default:
		return SQL_DEFAULT;
			break;
	}
}
