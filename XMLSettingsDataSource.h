#pragma once
#include"ISettingsDataSource.h"

namespace DrvFtaeAlarm {

	class XMLSettingsDataSource final : public ISettingsDataSource
	{
	public:
		XMLSettingsDataSource() = default;
		XMLSettingsDataSource(const XMLSettingsDataSource& src) = delete;
		XMLSettingsDataSource(XMLSettingsDataSource&& src) = delete;
		XMLSettingsDataSource& operator=(const XMLSettingsDataSource& rhs) = delete;
		XMLSettingsDataSource& operator=(XMLSettingsDataSource&& rhs) = delete;
		bool Save(const std::string& filterName, const std::vector<StatementCondition>& conditions, bool selected) override;
		bool Save(const ConnectionAttributes& attributes) override;
		bool Load(std::map<std::string, std::vector<StatementCondition> >& filters) override;
		bool Load(std::map<std::string, bool>& selectedFilters) override;
		bool Load(ConnectionAttributes& attributes) override;
		~XMLSettingsDataSource() {};
	};
}