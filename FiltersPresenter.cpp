#include"FiltersPresenter.h"

DrvFtaeAlarm::FiltersPresenter::FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database, const std::shared_ptr<ISettingsDataSource>& settingsDataSource) :view(), _database(database), _settingsDataSource(settingsDataSource)
{
	
}

DrvFtaeAlarm::FiltersPresenter::~FiltersPresenter()
{
	view.reset();
	_database.reset();
	_settingsDataSource.reset();
}

void DrvFtaeAlarm::FiltersPresenter::SetViewInput(std::shared_ptr<IFiltersViewInput> input)
{
	view = input;
}

void DrvFtaeAlarm::FiltersPresenter::viewIsReady()
{
	std::map< std::string, PropertyType> properties;
	std::pair<std::string, PropertyType> pair1 = std::make_pair<std::string, PropertyType>(std::string("columnName1"),PropertyType::PROPTYPE_TEXT);
	properties.insert(pair1);
	std::pair<std::string, PropertyType> pair2 = std::make_pair<std::string, PropertyType>(std::string("columnName2"), PropertyType::PROPTYPE_NUMERIC);
	properties.insert(pair2);
	std::pair<std::string, PropertyType> pair3 = std::make_pair<std::string, PropertyType>(std::string("columnName3"), PropertyType::PROPTYPE_BOOLEAN);
	properties.insert(pair3);
	std::pair<std::string, PropertyType> pair4 = std::make_pair<std::string, PropertyType>(std::string("columnName4"), PropertyType::PROPTYPE_DATE);
	properties.insert(pair4);
	std::pair<std::string, PropertyType> pair5 = std::make_pair<std::string, PropertyType>(std::string("columnName4"), PropertyType::PROPTYPE_TEXT);
	properties.insert(pair5);
	_settingsDataSource->Load(filters);
	std::shared_ptr<IFiltersViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->LoadPropertiesList(properties);
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

void DrvFtaeAlarm::FiltersPresenter::SaveFilters()
{
	bool bSaved = false;
	for (FiltersIterator itr = filters.begin(); itr != filters.end(); ++itr)
	{
		bSaved = _settingsDataSource->Save(itr->first, itr->second, true);
	}
}