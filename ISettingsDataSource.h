#pragma once
#include"StatementCondition.h"
#include"ConnectionAttributes.h"

#include<vector>
#include<map>
namespace DrvFtaeAlarm {

	class ISettingsDataSource {
	public:
		virtual bool Save(const std::string& filterName, const std::vector<StatementCondition>& conditions, bool selected) = 0;
		virtual bool Save(const ConnectionAttributes& attributes) = 0;
		virtual bool Load(std::map<std::string, std::vector<StatementCondition> >& filters) = 0;
		virtual bool Load(std::map<std::string, bool>& selectedFilters) = 0;
		virtual bool Load(ConnectionAttributes& attributes) = 0;
		virtual ~ISettingsDataSource() {};
	};

}

