#pragma once
#include"StatementCondition.h"
#include<vector>
#include<map>
namespace DrvFtaeAlarm {
	class IFiltersViewInput {
	public:
		virtual void setupInitialState() = 0;
		virtual void LoadConditionsList(const std::vector<StatementCondition>& conditions) = 0;
		virtual void LoadFiltersList(const std::vector<std::string>& filters) = 0;
		virtual void WarningMessage(std::string message) = 0;
		virtual void AddFilter(std::string filterName) = 0;
		virtual void AddCondition(const StatementCondition& condition) = 0;
		virtual void LoadPropertiesList(const std::map<std::string, PropertyType>& properties) = 0;
		virtual ~IFiltersViewInput() {};
	};
}