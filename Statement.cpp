#include"Statement.h"

DrvFtaeAlarm::Statement::Statement(const std::shared_ptr<Connection>& connection, const std::string& query, std::initializer_list<std::string> list) :ptrConnection(connection), strQuery(query), binders(list) {

}

DrvFtaeAlarm::Statement::Statement(const std::shared_ptr<Connection>& connection, const std::string& query) : Statement(connection, query, std::initializer_list<std::string>{}) {

}

DrvFtaeAlarm::Statement::Statement(std::shared_ptr<Connection>&& connection, std::string&& query, std::initializer_list<std::string> list) : ptrConnection(std::move(connection)), strQuery(std::move(query)), binders(list) {

}

DrvFtaeAlarm::Statement::Statement(std::shared_ptr<Connection>&& connection, std::string&& query) : Statement(std::move(connection), std::move(query), std::initializer_list<std::string>{}) {

}


DrvFtaeAlarm::Statement::Statement(const std::shared_ptr<Connection>& connection, const std::string& query, const std::vector<std::string>& parameters) : ptrConnection(connection), strQuery(query), binders(parameters)
{

}

DrvFtaeAlarm::Statement::Statement(const std::shared_ptr<Connection>& connection, const std::string& query, std::vector<std::string>&& parameters) : ptrConnection(connection), strQuery(query), binders(std::move(parameters))
{

}

DrvFtaeAlarm::Statement::Statement(std::shared_ptr<Connection>&& connection, std::string&& query, std::vector<std::string>&& parameters) : ptrConnection(std::move(connection)), strQuery(std::move(query)), binders(std::move(parameters))
{

}

DrvFtaeAlarm::Statement::~Statement() {
	ptrConnection.reset();
	strQuery.clear();
	binders.clear();
}

DrvFtaeAlarm::Statement::Statement(Statement&& src) noexcept {
	ptrConnection = src.ptrConnection;
	src.ptrConnection.reset();
	strQuery = std::move(src.strQuery);
	binders = std::move(src.binders);
}

DrvFtaeAlarm::Statement& DrvFtaeAlarm::Statement::operator=(Statement&& rhs) noexcept
{
	if (this == &rhs) {
		return *this;
	}
	else {
		ptrConnection = rhs.ptrConnection;
		rhs.ptrConnection.reset();
		strQuery = std::move(rhs.strQuery);
		binders = std::move(rhs.binders);
		return *this;
	}
}

std::string DrvFtaeAlarm::Statement::getQueryString() const {
	return strQuery;
}
