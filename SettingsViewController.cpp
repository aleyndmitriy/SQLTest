#include"SettingsViewController.h"
#include<algorithm>
#include"LoginInitializer.h"
#include"FiltersInitializer.h"

DrvFtaeAlarm::SettingsViewController::SettingsViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<ISettingsViewOutput>& output):UIDialogViewController(parent), presenter(output)
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
	LoginInitializer::CreateModule(GetModuleHandle(NULL), shared_from_this());
	TCITEM tia;
	tia.mask = TCIF_TEXT;
	wchar_t header1[] = L"Configuration";
	tia.pszText = header1;
	TabCtrl_InsertItem(hTabControl, 0, &tia);
	FiltersInitializer::CreateModule(GetModuleHandle(NULL), shared_from_this());
	wchar_t header2[] = L"Filters";
	tia.pszText = header2;
	TabCtrl_InsertItem(hTabControl, 1, &tia);
	TabCtrl_SetCurSel(hTabControl, 0);
	ShiftChildDialog();
	CreateLoginView();
	presenter->setViewInput(shared_from_this());

}

void DrvFtaeAlarm::SettingsViewController::ShiftChildDialog()
{
	for (std::vector<std::weak_ptr<UIDialogViewController> >::iterator itr = childs.begin(); itr != childs.end(); itr++) {
		std::shared_ptr<UIDialogViewController> ptr = itr->lock();
		if (ptr) {
			HWND* ptrHandle =  reinterpret_cast<HWND*>(ptr->GetInterface());
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
	EndDialog(window, 0);
	window = NULL;
}


void DrvFtaeAlarm::SettingsViewController::OnBtnOkTouched()
{
	CloseView();
}

void DrvFtaeAlarm::SettingsViewController::OnBtnCancelTouched()
{
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
	if (childs.at(0).expired()) {

	}
	else {
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
	if (childs.at(1).expired()) {

	}
	else {
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
		ptrHandle = reinterpret_cast<HWND*>(controller->GetInterface());
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

