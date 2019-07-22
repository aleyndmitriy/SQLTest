#include"SettingsPresenter.h"

DrvFtaeAlarm::SettingsPresenter::SettingsPresenter():view()
{

}

DrvFtaeAlarm::SettingsPresenter::~SettingsPresenter()
{
	view.reset();
}

void DrvFtaeAlarm::SettingsPresenter::setViewInput(std::shared_ptr<ISettingsViewInput> input)
{
	view = input;
}

void DrvFtaeAlarm::SettingsPresenter::viewIsReady()
{
	
}