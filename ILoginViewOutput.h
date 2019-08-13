#pragma once
#include<memory>
#include<string>
#include"ILoginViewInput.h"

namespace DrvFtaeAlarm {
	class ILoginViewOutput {
	public:
		virtual void viewIsReady() = 0;
		virtual void SetViewInput(std::shared_ptr<ILoginViewInput> input) = 0;
		virtual void GetServerName(std::string&& serverName) = 0;
		virtual void GetServerIndex(int index) = 0;
		virtual void GetLogin(std::string&& login) = 0;
		virtual void GetPassword(std::string&& password) = 0;
		virtual void GetDatabaseName(std::string&& databaseName) = 0;
		virtual void GetDatabaseIndex(int index) = 0;
		virtual void GetAuthType(int isSystem) = 0;
		virtual void ConnectToDriver() = 0;
		virtual void ConnectToServer() = 0;
		virtual void CheckConnectToDatabase() = 0;
		virtual void SaveSettings() = 0;
		virtual ~ILoginViewOutput() {};
	};
}