#include "ConnectionAttributes.h"

bool DrvFtaeAlarm::operator==(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs)
{
	return (lhs.driver == rhs.driver && lhs.serverName == rhs.serverName && lhs.loginName == rhs.loginName &&
		lhs.password == rhs.password && lhs.databaseName == rhs.databaseName);
}

bool DrvFtaeAlarm::operator!=(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs)
{
	return (lhs.driver != rhs.driver || lhs.serverName != rhs.serverName || lhs.loginName != rhs.loginName ||
		lhs.password != rhs.password || lhs.databaseName != rhs.databaseName);
}