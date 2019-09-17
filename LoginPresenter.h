#pragma once
#include"ILoginViewOutput.h"
#include"DatabaseEngine.h"
#include"ISettingsDataSource.h"

namespace DrvFtaeAlarm {
	class LoginPresenter final: public ILoginViewOutput {
	public:
		LoginPresenter(const std::shared_ptr<DatabaseEngine> &database, const std::shared_ptr<ISettingsDataSource>& settingsDataSource);
		void viewIsReady() override;
		void SetViewInput(std::shared_ptr<ILoginViewInput> input) override;
		 ~LoginPresenter();
		 void GetServerName(std::string&& serverName) override;
		 void GetLogin(std::string&& login) override;
		 void GetPassword(std::string&& password) override;
		 void GetDatabaseName(std::string&& databaseName) override;
		 void GetDatabaseIndex(int index) override;
		 void ConnectToDriver() override;
		 void ConnectToServer() override;
		 void CheckConnectToDatabase() override;
		 void SaveSettings() override;
		 void GetAuthType(int isSystem) override;
		 void GetReportType(bool isAlarm) override;
	private:
		std::weak_ptr<ILoginViewInput> view;
		std::shared_ptr<DatabaseEngine> _database;
		std::shared_ptr<ISettingsDataSource> _settingsDataSource;
		ConnectionAttributes attributes;
		std::vector<std::string> serverNames;
		std::vector<std::string> databaseNames;
		void CloseExistentConnection();
	};
}