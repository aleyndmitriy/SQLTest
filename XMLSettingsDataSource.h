#pragma once
#include"ISettingsDataSource.h"

namespace DrvFtaeAlarm {

	class XMLSettingsDataSource: public ISettingsDataSource
	{
	public:
		XMLSettingsDataSource() = default;
		XMLSettingsDataSource(const XMLSettingsDataSource& src) = delete;
		XMLSettingsDataSource(XMLSettingsDataSource&& src) = delete;
		XMLSettingsDataSource& operator=(const XMLSettingsDataSource& rhs) = delete;
		XMLSettingsDataSource& operator=(XMLSettingsDataSource&& rhs) = delete;
		bool Save(const std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >& filters) override;
		bool Save(const ConnectionAttributes& attributes) override;
		bool Load(std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >& filters) override;
		bool Load(ConnectionAttributes& attributes) override;
		~XMLSettingsDataSource() {};
		bool LoadXMLString(const char* source, size_t length);
		bool SaveXMLString(const char* fileName);
	};
}