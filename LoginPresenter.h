#pragma once
#include"ILoginViewOutput.h"
#include"DatabaseEngine.h"
namespace DrvFtaeAlarm {
	class LoginPresenter final: public ILoginViewOutput {
	public:
		LoginPresenter(const std::shared_ptr<DatabaseEngine> &database);
		LoginPresenter(std::shared_ptr<DatabaseEngine>&& database);
		void viewIsReady() override;
		void SetViewInput(std::shared_ptr<ILoginViewInput> input) override;
		 ~LoginPresenter();
		 void FindServerList() override;
		 void GetServerName(std::string&& serverName) override;
		 void GetServerIndex(int index) override;
		 void GetLogin(std::string&& login) override;
		 void GetPassword(std::string&& password) override;
		 void GetDatabaseName(std::string&& databaseName) override;
		 void GetDatabaseIndex(int index) override;
		 void ConnectToServer() override;
		 void ConnectToDatabase() override;
	private:
		std::weak_ptr<ILoginViewInput> view;
		std::shared_ptr<DatabaseEngine> _database;
		ConnectionAttributes attributes;
		std::vector<std::string> serverNames;
		std::vector<std::string> databaseNames;
	};
}