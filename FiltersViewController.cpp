#include"FiltersViewController.h"

DrvFtaeAlarm::FiltersViewController::FiltersViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<IFiltersViewOutput>& output): UIDialogViewController(parent), presenter(output)
{

}

void DrvFtaeAlarm::FiltersViewController::setupInitialState()
{
	presenter->SetViewInput(shared_from_this());
}


void DrvFtaeAlarm::FiltersViewController::AddChildViewController(const std::shared_ptr<UIDialogViewController>& child)
{

}

void DrvFtaeAlarm::FiltersViewController::AddToParentView()
{
	parentController->AddChildViewController(shared_from_this());
}

DrvFtaeAlarm::FiltersViewController::~FiltersViewController()
{
	presenter.reset();
	if (window != NULL) {
		DestroyWindow(window);
		window = 0;
	}
}

void DrvFtaeAlarm::FiltersViewController::CloseView()
{
	SendMessage(window, WM_CLOSE, 0, 0);
}

void DrvFtaeAlarm::FiltersViewController::ShowView()
{
	ShowWindow(window, SW_SHOW);
}

void DrvFtaeAlarm::FiltersViewController::HideView() {
	ShowWindow(window, SW_HIDE);
}


INT_PTR WINAPI FiltersDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iTab = 0;
	HWND* ptrHandle = NULL;
	static std::shared_ptr<DrvFtaeAlarm::FiltersViewController> controller;
	switch (uMsg) {
	case WM_INITDIALOG:
		controller = std::shared_ptr<DrvFtaeAlarm::FiltersViewController>(reinterpret_cast<DrvFtaeAlarm::FiltersViewController*>(lParam));
		ptrHandle = reinterpret_cast<HWND*>(controller->GetInterface());
		*ptrHandle = hwnd;
		controller->setupInitialState();
		controller->AddToParentView();
		break;
	case WM_CLOSE:
		controller.reset();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST_FILTERS:

			break;
		case IDC_LIST_CONDITIONS:

			break;
		case IDC_COMBO_PROPERTY:
			iTab = 0;
			break;
		case IDC_COMBO_PROPERTYTYPE:

			break;
		}
		break;
	case WM_NOTIFY:
		if (((LPNMHDR)lParam)->idFrom == IDC_TAB)
		{

		}
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	}

	return(FALSE);
}



