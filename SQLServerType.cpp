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

std::pair<SQLSMALLINT, SQLSMALLINT>  DrvFtaeAlarm::GetSQLServerTypeIdentifier(SQLServerType type)
{
	switch (type)
	{
	case SQLServerType::UNIQUEIDENTIFIER:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_GUID, SQL_GUID);
		break;
	case SQLServerType::NVARCHAR:
	case SQLServerType::VARCHAR:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_CHAR, SQL_VARCHAR);
		break;
	case SQLServerType::NTEXT:
	case SQLServerType::TEXT:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_CHAR, SQL_LONGVARCHAR);
		break;
	case SQLServerType::NCHAR:
	case SQLServerType::CHAR:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_CHAR, SQL_CHAR);
		break;
	case SQLServerType::DATE:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_TYPE_DATE, SQL_TYPE_DATE);
		break;
	case SQLServerType::TIME:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_TYPE_TIME, SQL_TYPE_TIME);
		break;
	case SQLServerType::DATETIME:
	case SQLServerType::DATETIME2:
	case SQLServerType::SMALLDATETIME:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP);
		break;
	case SQLServerType::DATETIMEOFFSET:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_INTERVAL_SECOND, SQL_INTERVAL_SECOND);
		break;
	case SQLServerType::BINARY:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_BINARY, SQL_BINARY);
		break;
	case SQLServerType::VARBINARY:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_BINARY, SQL_VARBINARY);
		break;
	case SQLServerType::NUMERIC:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_CHAR, SQL_NUMERIC);
		break;
	case SQLServerType::INT:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_SLONG, SQL_INTEGER);
		break;
	case SQLServerType::SMALLINT:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_SSHORT, SQL_SMALLINT);
		break;
	case SQLServerType::TINYINT:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_STINYINT, SQL_TINYINT);
		break;
	case SQLServerType::BIGINT:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_SBIGINT, SQL_BIGINT);
		break;
	case SQLServerType::BIT:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_BIT, SQL_BIT);
		break;
	case SQLServerType::REAL:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_FLOAT, SQL_REAL);
		break;
	case SQLServerType::FLOAT:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_DOUBLE, SQL_FLOAT);
		break;
	case SQLServerType::XML:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_CHAR, SQL_LONGVARCHAR);
		break;
	default:
		return std::make_pair<SQLSMALLINT, SQLSMALLINT>(SQL_C_DEFAULT, SQL_DEFAULT);
			break;
	}
}
