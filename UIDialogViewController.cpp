#include"UIDialogViewController.h"

DrvFtaeAlarm::UIDialogViewController::UIDialogViewController(const std::shared_ptr<UIDialogViewController>& parent) :parentController(parent),window(NULL)
{

}


DrvFtaeAlarm::UIDialogViewController::~UIDialogViewController()
{
	parentController.reset();
}

void* DrvFtaeAlarm::UIDialogViewController::GetInterface(int nIfcId)
{
	return &(this->window);
}