#pragma once
#include<string>
namespace DrvFtaeAlarm {

	struct ConnectionAttributes {
		std::string driver;
		std::string serverName;
		std::string databaseName;
		std::string loginName;
		std::string password;
	};

}
