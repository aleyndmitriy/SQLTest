#include"SQLServerStatement.h"
#include"Utils.h"
#include"Constants.h"
#include<tchar.h>

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

void DrvFtaeAlarm::SQLServerStatement::HandleDiagnosticRecord()
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	TCHAR       wszMessage[SQL_MAX_MESSAGE_LENGTH];
	TCHAR       wszState[SQL_SQLSTATE_SIZE + 1];
	while (SQLGetDiagRec(SQL_HANDLE_STMT, sqlStmt, ++iRec, reinterpret_cast<SQLCHAR*>(wszState), &iError, reinterpret_cast<SQLCHAR*>(wszMessage),
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(TCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS)
	{
		if (_tcsncmp(wszState, "01004", 5))
		{
			fprintf(stderr, "[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

void DrvFtaeAlarm::SQLServerStatement::allocateStatement()
{
	SQLSMALLINT res = SQLAllocHandle(SQL_HANDLE_STMT, ptrConnection->GetInterface(), &sqlStmt);
	if (res == SQL_ERROR)
	{
		HandleDiagnosticRecord();
		freeStatement();
		return;
	}

	for (std::vector<std::string>::const_iterator constItr = binders.cbegin(); constItr != binders.cend(); constItr++) {
		strQuery = FindAndReplace(strQuery, std::string("?"), *constItr);
	}

	res = SQLPrepare(sqlStmt, (SQLCHAR*)(strQuery.c_str()), SQL_NTS);
	if (!SQL_SUCCEEDED(res)) {
		HandleDiagnosticRecord();
		freeStatement();
		return;
	}
}

std::vector<DrvFtaeAlarm::Record> DrvFtaeAlarm::SQLServerStatement::Execute()
{
	std::vector<DrvFtaeAlarm::Record> recordSet = std::vector<DrvFtaeAlarm::Record>();
	SQLSMALLINT res = SQLExecute(sqlStmt);
	if (!SQL_SUCCEEDED(res)) {
		HandleDiagnosticRecord();
		freeStatement();
		return recordSet;
	}
	SQLSMALLINT shNumberOfColumns = 0;
	res = SQLNumResultCols(sqlStmt, &shNumberOfColumns);
	if (!SQL_SUCCEEDED(res)) {
		HandleDiagnosticRecord();
		freeStatement();
		return recordSet;
	}

	SQLSMALLINT     coltype = 0;
	SQLSMALLINT     colnamelen = 0;
	SQLULEN     collen[MAX_COLUMNS];
	SQLCHAR         colname[STR_LENGTH];
	SQLLEN      displaysize = 0;
	SQLSMALLINT     scale = 0;
	std::unique_ptr<DataBinding[]> bindingArray(new DataBinding[shNumberOfColumns]);

	for (SQLSMALLINT colIndex = 0; colIndex < shNumberOfColumns; colIndex++) {
		res = SQLDescribeCol(sqlStmt, colIndex + 1, colname, STR_LENGTH, &colnamelen, &coltype, collen + colIndex, &scale, NULL);
		HandleDiagnosticRecord();
		colname[colnamelen] = '\0';
		res = SQLColAttributes(sqlStmt, colIndex + 1, SQL_DESC_DISPLAY_SIZE, NULL, 0, NULL, &displaysize);
		//size_t len = wcslen(colname) + 1;
		std::string wideStr = std::string((TCHAR*)colname);
		bindingArray[colIndex].colName = wideStr;
		bindingArray[colIndex].dataType = coltype;
		bindingArray[colIndex].iBufferLength = max(displaysize, colnamelen);
		bindingArray[colIndex].ptrDataValue = std::unique_ptr<char[]>(new char[bindingArray[colIndex].iBufferLength]);
		res = SQLBindCol(sqlStmt, colIndex + 1, SQL_CHAR, bindingArray[colIndex].ptrDataValue.get(), bindingArray[colIndex].iBufferLength, &(bindingArray[colIndex].iStrLenOrIndex));
		if (!SQL_SUCCEEDED(res)) {
			freeStatement();
			return recordSet;
		}
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