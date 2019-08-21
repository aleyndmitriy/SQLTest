#include"FiltersPresenter.h"
#include "SQLServerType.h"

DrvFtaeAlarm::FiltersPresenter::FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database, const std::shared_ptr<DatabaseInfoDAO>& databaseInfoDAO, const std::shared_ptr<ISettingsDataSource>& settingsDataSource) :view(), _database(database),_databaseInfoDAO(databaseInfoDAO), _settingsDataSource(settingsDataSource)
{
	
}

DrvFtaeAlarm::FiltersPresenter::~FiltersPresenter()
{
	view.reset();
	_database.reset();
	_databaseInfoDAO.reset();
	_settingsDataSource.reset();
}

void DrvFtaeAlarm::FiltersPresenter::SetViewInput(std::shared_ptr<IFiltersViewInput> input)
{
	view = input;
}

void DrvFtaeAlarm::FiltersPresenter::viewIsReady()
{
	std::map< std::string, PropertyType> properties;
	ConnectionAttributes attributes;
	if (!attributes.serverName.empty() && !attributes.loginName.empty() && !attributes.password.empty()) {
		_settingsDataSource->Load(attributes);
		std::unique_ptr<SQLTable> table = _databaseInfoDAO->GetTableInfo(attributes, attributes.databaseName, std::string("ConditionEvent"));
		for (SQLTable::const_iterator itr = table->cbegin(); itr != table->cend(); ++itr) {
			std::pair<std::string, PropertyType> pair = std::make_pair<std::string, PropertyType>(std::string(itr->first), PropertyTypeFromString(itr->second));
			properties.insert(pair);
		}
	}
	
	std::map<std::pair<std::string, bool>, std::vector<StatementCondition> > loadFilters;
	_settingsDataSource->Load(loadFilters);
	for (std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >::const_iterator itr = loadFilters.cbegin(); itr != loadFilters.cend(); ++itr) {
		filters.insert(std::make_pair<std::string, ConditionsVector>(std::string(itr->first.first), ConditionsVector(itr->second)));
	}
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		if (!properties.empty()) {
			ptrView->LoadPropertiesList(properties);
		}
		for (FiltersIterator itr = filters.begin(); itr != filters.end(); ++itr)
		{
			ptrView->AddFilter(itr->first);
		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::AddCondition(StatementCondition&& condition, std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		FiltersIterator itr = filters.find(filterName);
		if (itr != filters.end()) {
			itr->second.push_back(condition);
			ptrView->AddCondition(condition, itr->second.size() - 1);
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));

		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::AddFilter(std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		FiltersIterator itr = filters.find(filterName);
		if (itr != filters.end()) {
			ptrView->WarningMessage(std::string("Filter with such name already exist!"));
		}
		else {
			std::pair<FiltersIterator, bool> res = filters.insert(std::make_pair<std::string, ConditionsVector>(std::string(filterName), ConditionsVector{}));
			if (res.second) {
				ptrView->AddFilter(filterName);
			}
			else {
				ptrView->WarningMessage(std::string("Error of filter inserting!"));
			}
		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::SelectFilter(std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		FiltersIterator itr = filters.find(filterName);
		if (itr != filters.end()) {
			int index = 0;
			for (std::vector<StatementCondition>::const_iterator statementItr = itr->second.cbegin(); statementItr != itr->second.cend(); ++statementItr)
			{
				ptrView->AddCondition(*statementItr, index++);
			}
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));
			
		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::SelectCondition(size_t index, std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		FiltersIterator itr = filters.find(filterName);
		if (itr != filters.end()) {
			if (index < itr->second.size()) {
				ptrView->SelectedCondition(itr->second.at(index));
			}
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));

		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::RemoveCondition(size_t index, std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		FiltersIterator itr = filters.find(filterName);
		if (itr != filters.end()) {
			if (index < itr->second.size()) {
				int searchIndex = 0;
				std::vector<StatementCondition>::const_iterator statementItr = itr->second.cbegin();
				while (searchIndex < index)
				{
					++statementItr;
					++searchIndex;
				}
				itr->second.erase(statementItr);
			}
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));

		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::RemoveAllConditions(std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		FiltersIterator itr = filters.find(filterName);
		if (itr != filters.end()) {
				itr->second.clear();
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));

		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::RemoveFilter(std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		FiltersIterator itr = filters.find(filterName);
		if (itr != filters.end()) {
			filters.erase(itr);
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));

		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::SaveFilters()
{
	bool bSaved = false;
	std::map<std::pair<std::string, bool>, ConditionsVector> savingFilters;
	for (FiltersIterator itr = filters.begin(); itr != filters.end(); ++itr)
	{
		std::pair<std::pair<std::string, bool>, ConditionsVector> val = 
			std::make_pair<std::pair<std::string, bool>, ConditionsVector>(std::make_pair<std::string, bool>(std::string(itr->first), true), ConditionsVector(itr->second));
		savingFilters.insert(val);
	}
	bSaved = _settingsDataSource->Save(savingFilters);
}

void DrvFtaeAlarm::FiltersPresenter::LoadProperties() {
	
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		ConnectionAttributes attributes;
		_settingsDataSource->Load(attributes);
		if (!attributes.serverName.empty() && !attributes.databaseName.empty()) {
			std::map< std::string, PropertyType> properties;
			std::unique_ptr<SQLTable> table = _databaseInfoDAO->GetTableInfo(attributes, attributes.databaseName, std::string("ConditionEvent"));
			if (table) {
				for (SQLTable::const_iterator itr = table->cbegin(); itr != table->cend(); ++itr) {
					std::pair<std::string, PropertyType> pair = std::make_pair<std::string, PropertyType>(std::string(itr->first), PropertyTypeFromString(itr->second));
					properties.insert(pair);
				}	
			}
			ptrView->LoadPropertiesList(properties);
		}
		ptrView->StopLoading();
	}
	
}