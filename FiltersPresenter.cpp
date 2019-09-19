#include"FiltersPresenter.h"
#include "SQLServerType.h"

DrvFtaeAlarm::FiltersPresenter::FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database, const std::shared_ptr<DatabaseInfoDAO>& databaseInfoDAO, std::shared_ptr<ConnectionAttributes> externAttributes, std::shared_ptr<std::map<std::string, std::vector<StatementCondition> > > externalFilters) :_view(), _database(database),_databaseInfoDAO(databaseInfoDAO), _externAttributes(externAttributes), _externalFilters(externalFilters)
{
	
}

DrvFtaeAlarm::FiltersPresenter::~FiltersPresenter()
{
	_view.reset();
	_database.reset();
	_databaseInfoDAO.reset();
	_externAttributes.reset();
	_externalFilters.reset();
}

void DrvFtaeAlarm::FiltersPresenter::SetViewInput(std::shared_ptr<IFiltersViewInput> input)
{
	_view = input;
}

void DrvFtaeAlarm::FiltersPresenter::viewIsReady()
{
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		for (FiltersIterator itr = _externalFilters->begin(); itr != _externalFilters->end(); ++itr)
		{
			ptrView->AddFilter(itr->first);
		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::AddCondition(StatementCondition&& condition, std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		FiltersIterator itr = _externalFilters->find(filterName);
		if (itr != _externalFilters->end()) {
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
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		if (filterName.empty()) {
			ptrView->ErrorMessage(std::string("Please, enter filter name!"));
		}
		FiltersIterator itr = _externalFilters->find(filterName);
		if (itr != _externalFilters->end()) {
			ptrView->WarningMessage(std::string("Filter with such name already exist!"));
		}
		else {
			std::pair<FiltersIterator, bool> res = _externalFilters->insert(std::make_pair<std::string, ConditionsVector>(std::string(filterName), ConditionsVector{}));
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
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		FiltersIterator itr = _externalFilters->find(filterName);
		if (itr != _externalFilters->end()) {
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
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		FiltersIterator itr = _externalFilters->find(filterName);
		if (itr != _externalFilters->end()) {
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
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		FiltersIterator itr = _externalFilters->find(filterName);
		if (itr != _externalFilters->end()) {
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
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		FiltersIterator itr = _externalFilters->find(filterName);
		if (itr != _externalFilters->end()) {
				itr->second.clear();
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));

		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::RemoveFilter(std::string filterName)
{
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		FiltersIterator itr = _externalFilters->find(filterName);
		if (itr != _externalFilters->end()) {
			_externalFilters->erase(itr);
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));

		}
	}
}

void DrvFtaeAlarm::FiltersPresenter::SaveFilters()
{
	
}

void DrvFtaeAlarm::FiltersPresenter::LoadProperties() {
	
	std::shared_ptr<IFiltersViewInput> ptrView = _view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		if (!_externAttributes->serverName.empty() && !_externAttributes->databaseName.empty()) {
			std::map< std::string, PropertyType> properties;
			std::unique_ptr<SQLTable> table = _databaseInfoDAO->GetTableInfo(false, *_externAttributes, _externAttributes->databaseName, std::string("ConditionEvent"));
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