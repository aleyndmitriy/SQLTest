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
		virtual bool LoadSettingsString(const char* source, size_t length) = 0;
		virtual bool SaveSettingsString(const char* fileName) = 0;
		virtual std::vector<std::string> GetFiltersName() = 0;
		virtual ~ISettingsDataSource() {};
	};

}

