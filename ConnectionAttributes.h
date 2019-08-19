#pragma once
#include<string>
namespace DrvFtaeAlarm {
	struct ConnectionAttributes {
		std::string driver;
		std::string serverName;
		std::string databaseName;
		std::string loginName;
		std::string password;
		bool isServerAuthentication;
		bool isAlarmReport;
		ConnectionAttributes();
		~ConnectionAttributes();
	};

	bool operator==(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs);
	bool operator!=(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs);
}
