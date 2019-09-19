#include"SettingsViewController.h"
#include<algorithm>
#include"LoginInitializer.h"
#include"FiltersInitializer.h"

DrvFtaeAlarm::SettingsViewController::SettingsViewController(const std::shared_ptr<UIDialogViewController>& parent, std::function<ODS::UI::IAbstractUIFacrory* (void)> factoryGetter, const std::shared_ptr<ISettingsViewOutput>& output, std::shared_ptr<ConnectionAttributes> attributes, std::shared_ptr<std::map<std::string, std::vector<StatementCondition> > > filters):UIDialogViewController(parent), presenter(output), uiFactoryGetter(factoryGetter), isOk(false),connectionAttributes(attributes),conditionFilters(filters)
{
	
}

void DrvFtaeAlarm::SettingsViewController::setupInitialState()
{
	INITCOMMONCONTROLSEX iccex;
	DWORD dwDLGBase = GetDialogBaseUnits();
	int cxMargin = LOWORD(dwDLGBase) / 4;
	int cyMargin = HIWORD(dwDLGBase) / 8;
	iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccex.dwICC = ICC_TAB_CLASSES;
	InitCommonControlsEx(&iccex);
	HWND hTabControl = GetDlgItem(window, IDC_TAB);
	LoginInitializer::CreateModule(GetModuleHandle("Drv_Ftae_HdaAlarm.dll"), uiFactoryGetter, shared_from_this(), connectionAttributes);
	TCITEM tia;
	tia.mask = TCIF_TEXT;
	TCHAR header1[] = "Configuration";
	tia.pszText = header1;
	TabCtrl_InsertItem(hTabControl, 0, &tia);
	FiltersInitializer::CreateModule(GetModuleHandle("Drv_Ftae_HdaAlarm.dll"), shared_from_this(), connectionAttributes, conditionFilters);
	TCHAR header2[] = "Filters";
	tia.pszText = header2;
	TabCtrl_InsertItem(hTabControl, 1, &tia);
	TabCtrl_SetCurSel(hTabControl, 0);
	ShiftChildDialog();
	DestroyFiltersView();
	CreateLoginView();
	presenter->setViewInput(shared_from_this());

}

void DrvFtaeAlarm::SettingsViewController::ShiftChildDialog()
{
	for (std::vector<std::weak_ptr<UIDialogViewController> >::iterator itr = childs.begin(); itr != childs.end(); itr++) {
		std::shared_ptr<UIDialogViewController> ptr = itr->lock();
		if (ptr) {
			HWND* ptrHandle =  reinterpret_cast<HWND*>(ptr->GetInterface(0));
			RECT childRect;
			POINT point; 
			GetWindowRect(*ptrHandle,&childRect);
			point.x = childRect.left + 20;
			point.y = childRect.top + 40;
			int width = childRect.right - childRect.left - 10;
			int height = childRect.bottom - childRect.top - 10;
			HWND hTabControl = GetDlgItem(window, IDC_TAB);
			ScreenToClient(hTabControl, &point);
			//MoveWindow(*ptrHandle, point.x, point.y, width, height,TRUE);
			SetWindowPos(*ptrHandle, NULL, point.x, point.y, width, height, SWP_SHOWWINDOW);
		}
	}
}

void DrvFtaeAlarm::SettingsViewController::AddChildViewController(const std::shared_ptr<UIDialogViewController>& child)
{
	std::vector< std::weak_ptr<UIDialogViewController> >::iterator newEnd = std::remove_if(childs.begin(), childs.end(), [](std::weak_ptr<UIDialogViewController> ptr) {
		return ptr.expired(); });
	childs.erase(newEnd, childs.end());
	childs.push_back(child);
}

void DrvFtaeAlarm::SettingsViewController::AddToParentView()
{

}

DrvFtaeAlarm::SettingsViewController::~SettingsViewController()
{
	presenter.reset();
	childs.clear();
	EndDialog(window, isOk);
	isOk = false;
	window = NULL;
}


void DrvFtaeAlarm::SettingsViewController::OnBtnOkTouched()
{
	isOk = true;
	CloseView();
}

void DrvFtaeAlarm::SettingsViewController::OnBtnCancelTouched()
{
	isOk = false;
	CloseView();
}

void DrvFtaeAlarm::SettingsViewController::DestroyLoginView()
{
	std::shared_ptr<UIDialogViewController> ptr = childs.at(0).lock();
	if (ptr) {
		ptr->HideView();
	}
}

void DrvFtaeAlarm::SettingsViewController::CreateLoginView()
{
	if (childs.at(0).expired() == false) {
		std::shared_ptr<UIDialogViewController> ptr = childs.at(0).lock();
		if (ptr) {
			ptr->ShowView();
		}
	}
}


void DrvFtaeAlarm::SettingsViewController::DestroyFiltersView()
{
	std::shared_ptr<UIDialogViewController> ptr = childs.at(1).lock();
	if (ptr) {
		ptr->HideView();
	}
}

void DrvFtaeAlarm::SettingsViewController::CreateFiltersView()
{
	if (childs.at(1).expired() == false) {
		std::shared_ptr<UIDialogViewController> ptr = childs.at(1).lock();
		if (ptr) {
			ptr->ShowView();
		}
	}
}

void DrvFtaeAlarm::SettingsViewController::CloseView()
{
	for (std::vector<std::weak_ptr<UIDialogViewController> >::iterator itr = childs.begin(); itr != childs.end(); itr++) {
		std::shared_ptr<UIDialogViewController> ptr = itr->lock();
		if (ptr) {
			ptr->CloseView();
		}
	}
	SendMessage(window, WM_CLOSE, 0, 0);
}


void DrvFtaeAlarm::SettingsViewController::ShowView()
{
	ShowWindow(window, SW_SHOW);
}

void DrvFtaeAlarm::SettingsViewController::HideView() {
	ShowWindow(window, SW_HIDE);
}

INT_PTR WINAPI SettingDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int iTab = 0;
	HWND* ptrHandle = NULL;
	static std::shared_ptr<DrvFtaeAlarm::SettingsViewController> controller;
	switch (uMsg) {
	case WM_INITDIALOG:
		controller = std::shared_ptr<DrvFtaeAlarm::SettingsViewController>(reinterpret_cast<DrvFtaeAlarm::SettingsViewController*>(lParam));
		ptrHandle = reinterpret_cast<HWND*>(controller->GetInterface(0));
		*ptrHandle = hwnd;
		controller->setupInitialState();
		break;
	case WM_CLOSE:
		controller.reset();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			controller->OnBtnOkTouched();
			break;
		case IDCANCEL:
			controller->OnBtnCancelTouched();
			break;
		}
		break;
	case WM_NOTIFY:
		if(((LPNMHDR)lParam)->idFrom == IDC_TAB)
		{
			switch(((LPNMHDR)lParam)->code)
			{
			case TCN_SELCHANGING:
				iTab = TabCtrl_GetCurSel(GetDlgItem(hwnd, IDC_TAB));
				if (iTab) {
					controller->DestroyFiltersView();
				}
				else {
					controller->DestroyLoginView();
				}
				break;
			case TCN_SELCHANGE:
				iTab = TabCtrl_GetCurSel(GetDlgItem(hwnd, IDC_TAB));
				if (iTab) {
					controller->CreateFiltersView();
				}
				else {
					controller->CreateLoginView();
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	}

	return(FALSE);
}

