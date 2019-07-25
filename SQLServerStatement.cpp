#include"SQLServerStatement.h"
#include"Utils.h"
#include"Constants.h"

DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(const std::shared_ptr<SQLServerConnection>& connection, const std::string& query, const std::vector<std::string>& parameters) : Statement(connection, query, parameters), sqlStmt(SQL_NULL_HSTMT)
{
	allocateStatement();
}


DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(const std::shared_ptr<SQLServerConnection>& connection, const std::string& query, std::vector<std::string>&& parameters) : Statement(connection, query, std::move(parameters)), sqlStmt(SQL_NULL_HSTMT)
{
	allocateStatement();
}

DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(SQLServerStatement&& src) noexcept : Statement(std::move(src)), sqlStmt(src.sqlStmt) {
	src.sqlStmt = SQL_NULL_HSTMT;
}

DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(std::shared_ptr<SQLServerConnection>&& connection, std::string&& query, std::vector<std::string>&& parameters) : Statement(std::move(connection), std::move(query), std::move(parameters)), sqlStmt(SQL_NULL_HSTMT)
{
	allocateStatement();
}

DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(std::shared_ptr<SQLServerConnection>&& connection, std::string&& query, std::initializer_list<std::string> parameters) : SQLServerStatement(std::move(connection), std::move(query), std::vector<std::string>(parameters))
{

}

DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(const std::shared_ptr<SQLServerConnection>& connection, const std::string& query, std::initializer_list<std::string> parameters) : SQLServerStatement(connection, query, std::vector<std::string>(parameters))
{

}

DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(const std::shared_ptr<SQLServerConnection>& connection, const std::string& query) : SQLServerStatement(connection, query, std::vector<std::string>())
{

}

DrvFtaeAlarm::SQLServerStatement::SQLServerStatement(std::shared_ptr<SQLServerConnection>&& connection, std::string&& query) : SQLServerStatement(std::move(connection), std::move(query), std::vector<std::string>())
{

}


DrvFtaeAlarm::SQLServerStatement& DrvFtaeAlarm::SQLServerStatement::operator=(SQLServerStatement&& rhs) noexcept {
	if (this == &rhs)
	{
		return *this;
	}
	else {
		sqlStmt = rhs.sqlStmt;
		rhs.sqlStmt = SQL_NULL_HSTMT;
		this->Statement::operator=(std::move(rhs));
		return *this;
	}
}

DrvFtaeAlarm::SQLServerStatement::~SQLServerStatement() {
	freeStatement();
}

void* DrvFtaeAlarm::SQLServerStatement::GetInterface() {
	return sqlStmt;
}

void DrvFtaeAlarm::SQLServerStatement::freeStatement() {
	SQLSMALLINT res = SQL_SUCCESS;
	if (sqlStmt)
	{
		res = SQLFreeHandle(SQL_HANDLE_STMT, sqlStmt);
		sqlStmt = SQL_NULL_HSTMT;
		ptrConnection.reset();
		strQuery.clear();

	}
}

void DrvFtaeAlarm::SQLServerStatement::allocateStatement()
{
	SQLSMALLINT res = SQLAllocHandle(SQL_HANDLE_STMT, ptrConnection->GetInterface(), &sqlStmt);
	if (res == SQL_ERROR)
	{
		freeStatement();
		return;
	}

	for (std::vector<std::string>::const_iterator constItr = binders.cbegin(); constItr != binders.cend(); constItr++) {
		strQuery = FindAndReplace(strQuery, std::string("?"), *constItr);
	}

	int sizeWcharbuff = MultiByteToWideChar(CP_ACP, 0, strQuery.c_str(), -1, NULL, 0);
	if (sizeWcharbuff < 0)
	{
		freeStatement();
		return;
	}
	std::unique_ptr<wchar_t[]> ptrQuery = std::make_unique<wchar_t[]>(sizeWcharbuff);
	sizeWcharbuff = MultiByteToWideChar(CP_ACP, 0, strQuery.c_str(), -1, ptrQuery.get(), sizeWcharbuff);
	if (sizeWcharbuff < 0)
	{
		freeStatement();
		return;
	}
	res = SQLPrepare(sqlStmt, ptrQuery.get(), SQL_NTS);
	if (!SQL_SUCCEEDED(res)) {
		freeStatement();
		return;
	}
}

std::vector<DrvFtaeAlarm::Record> DrvFtaeAlarm::SQLServerStatement::Execute()
{
	std::vector<DrvFtaeAlarm::Record> recordSet = std::vector<DrvFtaeAlarm::Record>();
	SQLSMALLINT shNumberOfColumns = 0;
	SQLSMALLINT res = SQLNumResultCols(sqlStmt, &shNumberOfColumns);
	if (!SQL_SUCCEEDED(res)) {
		freeStatement();
		return recordSet;
	}

	SQLSMALLINT     coltype = 0;
	SQLSMALLINT     colnamelen = 0;
	SQLULEN     collen[MAX_COLUMNS];
	SQLWCHAR         colname[STR_LENGTH];
	SQLLEN      displaysize = 0;
	SQLSMALLINT     scale = 0;
	std::unique_ptr<DataBinding[]> bindingArray(new DataBinding[shNumberOfColumns]);

	for (SQLSMALLINT colIndex = 0; colIndex < shNumberOfColumns; colIndex++) {
		res = SQLDescribeCol(sqlStmt, colIndex + 1, colname, STR_LENGTH, &colnamelen, &coltype, collen + colIndex, &scale, NULL);
		colname[colnamelen] = L'\0';
		res = SQLColAttributes(sqlStmt, colIndex + 1, SQL_DESC_DISPLAY_SIZE, NULL, 0, NULL, &displaysize);
		//size_t len = wcslen(colname) + 1;
		std::wstring wideStr = std::wstring(colname);
		bindingArray[colIndex].colName = Wstr2Str(wideStr);
		bindingArray[colIndex].dataType = coltype;
		bindingArray[colIndex].iBufferLength = max(displaysize, colnamelen);
		bindingArray[colIndex].ptrDataValue = std::unique_ptr<char[]>(new char[bindingArray[colIndex].iBufferLength]);
		res = SQLBindCol(sqlStmt, colIndex + 1, SQL_CHAR, bindingArray[colIndex].ptrDataValue.get(), bindingArray[colIndex].iBufferLength, &(bindingArray[colIndex].iStrLenOrIndex));
		if (!SQL_SUCCEEDED(res)) {
			freeStatement();
			return recordSet;
		}
	}
	res = SQLExecute(sqlStmt);
	if (!SQL_SUCCEEDED(res)) {
		freeStatement();
		return recordSet;
	}
	while ((res = SQLFetch(sqlStmt)) == SQL_SUCCESS)
	{
		Record record;
		for (SQLSMALLINT colIndex = 0; colIndex < shNumberOfColumns; colIndex++) {
			std::string dataString;
			if (bindingArray[colIndex].iStrLenOrIndex != SQL_NO_DATA) {
				dataString = std::string(bindingArray[colIndex].ptrDataValue.get());
			}
			record.insert(bindingArray[colIndex].colName, bindingArray[colIndex].dataType, dataString);
		}
		recordSet.push_back(record);
	}
	return recordSet;
}