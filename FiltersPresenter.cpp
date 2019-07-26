#include"FiltersPresenter.h"

DrvFtaeAlarm::FiltersPresenter::FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database) :view(), _database(database),selectedFilter(std::string()),selectedCondition(-1)
{
	
}

DrvFtaeAlarm::FiltersPresenter::~FiltersPresenter()
{
	view.reset();
}

void DrvFtaeAlarm::FiltersPresenter::SetViewInput(std::shared_ptr<IFiltersViewInput> input)
{
	view = input;
}

void DrvFtaeAlarm::FiltersPresenter::viewIsReady()
{

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
			ptrView->LoadConditionsList(itr->second);
		}
		else {
			ptrView->WarningMessage(std::string("There is not such filter in list!"));
			
		}
	}
}