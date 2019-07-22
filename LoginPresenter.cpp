#include"LoginPresenter.h"

DrvFtaeAlarm::LoginPresenter::LoginPresenter():view()
{

}

DrvFtaeAlarm::LoginPresenter::~LoginPresenter() 
{
	view.reset();
}

void DrvFtaeAlarm::LoginPresenter::SetViewInput(std::shared_ptr<ILoginViewInput> input)
{
	view = input;
}

void DrvFtaeAlarm::LoginPresenter::viewIsReady()
{

}