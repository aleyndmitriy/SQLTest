#include"FiltersPresenter.h"

DrvFtaeAlarm::FiltersPresenter::FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database) :view(), _database(database)
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