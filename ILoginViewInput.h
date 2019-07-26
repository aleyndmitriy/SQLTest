#pragma once
#include<vector>
#include<string>
namespace DrvFtaeAlarm {
	class ILoginViewInput {
	public:
		virtual void setupInitialState() = 0;
		virtual void LoadServerList(const std::vector<std::string>& servers) = 0;
		virtual void LoadDatabasesList(const std::vector<std::string>& databases) = 0;
		virtual void WarningMessage(std::string message) = 0;
		virtual ~ILoginViewInput() {};

	};
}