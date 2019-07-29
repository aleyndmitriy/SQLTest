#include "XMLSettingsDataSource.h"

bool DrvFtaeAlarm::XMLSettingsDataSource::Save(const std::string& filterName, const std::vector<StatementCondition>& conditions, bool selected)
{
	return true;
}

bool DrvFtaeAlarm::XMLSettingsDataSource::Save(const ConnectionAttributes& attributes)
{
	return true;
}

bool DrvFtaeAlarm::XMLSettingsDataSource::Load(std::map<std::string, std::vector<StatementCondition> >& filters)
{
	return true;
}

bool DrvFtaeAlarm::XMLSettingsDataSource::Load(std::map<std::string, bool>& selectedFilters)
{
	return true;
}

bool DrvFtaeAlarm::XMLSettingsDataSource::Load(ConnectionAttributes& attributes)
{
	return true;
}