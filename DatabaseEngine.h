#pragma once
#include<vector>
#include<string>
#include"ConnectionAttributes.h"

namespace DrvFtaeAlarm {
	class DatabaseEngine
	{
	public:
		enum class AuthenticationType
		{
			System = 1,
			Server = 2
		};
		virtual void OpenConnection() = 0;
		virtual void OpenConnection(const ConnectionAttributes& attributes) = 0;
		virtual void loadServerInstances(std::string driverName) = 0;
		virtual void loadDatabaseInstances(std::string serverName, AuthenticationType type, std::string user, std::string password) = 0;
		virtual bool ChooseDatabase(std::string databaseName) = 0;
		virtual void CloseConnection() = 0;
		virtual std::vector<std::string> GetServersList() const = 0;
		virtual std::vector<std::string> GetDatabasesList() const = 0;
		virtual ~DatabaseEngine() {};
	};
}