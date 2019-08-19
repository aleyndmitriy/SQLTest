#include "ConnectionAttributes.h"

DrvFtaeAlarm::ConnectionAttributes::ConnectionAttributes():driver(),serverName(),databaseName(),loginName(),password(),isServerAuthentication(false),isAlarmReport(false)
{

}
DrvFtaeAlarm::ConnectionAttributes::~ConnectionAttributes()
{

}

bool DrvFtaeAlarm::operator==(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs)
{
	return (lhs.driver == rhs.driver && lhs.serverName == rhs.serverName && lhs.loginName == rhs.loginName &&
		lhs.password == rhs.password && lhs.databaseName == rhs.databaseName && lhs.isServerAuthentication == rhs.isServerAuthentication);
}

bool DrvFtaeAlarm::operator!=(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs)
{
	return (lhs.driver != rhs.driver || lhs.serverName != rhs.serverName || lhs.loginName != rhs.loginName ||
		lhs.password != rhs.password || lhs.databaseName != rhs.databaseName || lhs.isServerAuthentication != rhs.isServerAuthentication);
}