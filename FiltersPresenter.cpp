#include"FiltersPresenter.h"

DrvFtaeAlarm::FiltersPresenter::FiltersPresenter() :view()
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