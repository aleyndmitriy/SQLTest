#include"FiltersViewController.h"

DrvFtaeAlarm::FiltersViewController::FiltersViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<IFiltersViewOutput>& output): UIDialogViewController(parent), presenter(output)
{

}

void DrvFtaeAlarm::FiltersViewController::setupInitialState()
{
	presenter->SetViewInput(shared_from_this());

	INITCOMMONCONTROLSEX icex; 
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	CreateFiltersList();
	CreateConditionsList();
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

void DrvFtaeAlarm::FiltersViewController::LoadConditionsList(const std::vector<StatementCondition>& conditions)
{

}
void DrvFtaeAlarm::FiltersViewController::LoadFiltersList(const std::vector<std::string>& filters)
{

}

void DrvFtaeAlarm::FiltersViewController::CreateFiltersList()
{
	LVCOLUMN col;
	WCHAR wcColumnText[12] = TEXT("Filter Name");
	HWND hFilterListControl = GetDlgItem(window, IDC_LIST_FILTERS);
	RECT filterListRect;
	GetWindowRect(hFilterListControl,&filterListRect);

	memset(&col, 0, sizeof(col));
	col.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.pszText = wcColumnText;
	col.cx = filterListRect.right - filterListRect.left;
	int ind = ::SendMessage(hFilterListControl, LVM_INSERTCOLUMN, 0, (LPARAM)(&col));
	ListView_SetItemState(hFilterListControl, 0, LVIS_SELECTED, LVIS_SELECTED);
}

void DrvFtaeAlarm::FiltersViewController::CreateConditionsList()
{
	LVCOLUMN col;
	WCHAR wcColumnText[3][10] = { TEXT("Operation"),TEXT("Property"),TEXT("Condition") };
	HWND hConditionListControl = GetDlgItem(window, IDC_LIST_CONDITIONS);
	RECT conditionListRect;
	GetWindowRect(hConditionListControl, &conditionListRect);
	for (int i = 0; i < 3; i++)
	{
		memset(&col, 0, sizeof(col));
		col.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
		col.fmt = LVCFMT_LEFT;
		col.pszText = wcColumnText[i];
		col.cx = (conditionListRect.right - conditionListRect.left)/3;
		int ind = ::SendMessage(hConditionListControl, LVM_INSERTCOLUMN, i, (LPARAM)(&col));
	}
	ListView_SetItemState(hConditionListControl, 0, LVIS_SELECTED, LVIS_SELECTED);
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
	UINT_PTR uNotifyParam = 0;
	UINT uNotifyCode = 0;
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
		case IDC_COMBO_PROPERTY:
			iTab = 0;
			break;
		case IDC_COMBO_PROPERTYTYPE:

			break;
		case IDC_COMBO_CONDITION:

			break;
		case IDC_EDIT_VALUE1:

			break;
		case IDC_EDIT_VALUE2:

			break;
		case IDC_RADIO_AND:

			break;
		case IDC_RADIO_OR:

			break;
		case IDC_EDIT_INSTANCE:

			break;
		case IDC_BUTTON_ADDCONDITION:

			break;
		case IDC_BUTTON_REMOVECONDITION:

			break;
		case IDC_BUTTON_REMOVEALLCONDITIONS:

			break;
		case IDC_BUTTON_ADDFILTER:

			break;
		case IDC_BUTTON_REMOVEFILTER:

			break;
		default:

			break;
		}
		break;
	case WM_NOTIFY:
		uNotifyParam = ((LPNMHDR)lParam)->idFrom;
		uNotifyCode = ((LPNMHDR)lParam)->code;
		switch (uNotifyParam)
		{
		case IDC_LIST_FILTERS:

			break;
		case IDC_LIST_CONDITIONS:
			switch (uNotifyCode)
			{
			case LVN_GETDISPINFO:
				NMLVDISPINFO* plvdi = (NMLVDISPINFO*)lParam;

				break;
			}

			break;
		default:

			break;

		}
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	}

	return(FALSE);
}



