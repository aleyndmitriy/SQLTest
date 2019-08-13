#pragma once
#include<string>
namespace DrvFtaeAlarm {
	struct ConnectionAttributes {
		std::string driver;
		std::string serverName;
		std::string databaseName;
		std::string loginName;
		std::string password;
		bool isSystemAuthentication;
	};

	bool operator==(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs);
	bool operator!=(const ConnectionAttributes& lhs, const ConnectionAttributes& rhs);
}
