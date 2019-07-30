#pragma once
#include"StatementCondition.h"
#include"ConnectionAttributes.h"

#include<vector>
#include<map>
namespace DrvFtaeAlarm {

	class ISettingsDataSource {
	public:
		virtual bool Save(const std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >& filters) = 0;
		virtual bool Save(const ConnectionAttributes& attributes) = 0;
		virtual bool Load(std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >& filters) = 0;
		virtual bool Load(ConnectionAttributes& attributes) = 0;
		virtual ~ISettingsDataSource() {};
	};

}

