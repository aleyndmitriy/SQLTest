#include"Connection.h"

DrvFtaeAlarm::Connection::Connection(const std::shared_ptr<IEnvironment>& environment): ptrEnvironment(environment) {

}

DrvFtaeAlarm::Connection::Connection(const std::shared_ptr<IEnvironment>& environment, const ConnectionAttributes& attributes) : ptrEnvironment(environment), connectionAttributes(attributes) {

}

DrvFtaeAlarm::Connection::Connection(std::shared_ptr<IEnvironment>&& environment, ConnectionAttributes&& attributes) : ptrEnvironment(std::move(environment)), connectionAttributes(std::move(attributes)) {

}

DrvFtaeAlarm::Connection::Connection(const std::shared_ptr<IEnvironment>& environment, ConnectionAttributes&& attributes) : ptrEnvironment(environment), connectionAttributes(std::move(attributes)) {

}

DrvFtaeAlarm::Connection::Connection(std::shared_ptr<IEnvironment>&& environment) : ptrEnvironment(std::move(environment)) {

}

DrvFtaeAlarm::ConnectionAttributes DrvFtaeAlarm::Connection::GetConnectionAttributes() const
{
	return connectionAttributes;
}

DrvFtaeAlarm::Connection::~Connection() {
	ptrEnvironment.reset();
}