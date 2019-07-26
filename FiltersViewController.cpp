#include"FiltersViewController.h"
#include"Utils.h"
#include"Constants.h"

//https://stackoverflow.com/questions/26323468/items-and-subitems-in-list-view-control

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
	EnableConditionView(FALSE);
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
	for (std::vector<StatementCondition>::const_iterator itr = conditions.begin(); itr != conditions.end(); ++itr)
	{

	}
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
	GetWindowRect(hFilterListControl, &filterListRect);

	memset(&col, 0, sizeof(col));
	col.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.pszText = wcColumnText;
	col.cx = filterListRect.right - filterListRect.left;
	int ind = ::SendMessage(hFilterListControl, LVM_INSERTCOLUMN, 0, (LPARAM)(&col));
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
		int ind = SendMessage(hConditionListControl, LVM_INSERTCOLUMN, i, (LPARAM)(&col));
	}
	//ListView_SetItemState(hConditionListControl, 0, LVIS_SELECTED, LVIS_SELECTED);
}


void DrvFtaeAlarm::FiltersViewController::AddFilter()
{
	WCHAR wchFilterName[STR_LENGTH];
	GetDlgItemText(window, IDC_EDIT_INSTANCE, wchFilterName, STR_LENGTH);
	presenter->AddFilter(Wstr2Str(std::wstring(wchFilterName)));
}

void DrvFtaeAlarm::FiltersViewController::RemoveFilter()
{

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


void DrvFtaeAlarm::FiltersViewController::WarningMessage(std::string message)
{

}

void DrvFtaeAlarm::FiltersViewController::AddFilter(std::string filterName)
{
	HWND hFilterListControl = GetDlgItem(window, IDC_LIST_FILTERS);
	LVITEM item;
	wchar_t wchfilterName[STR_LENGTH];
	
	std::wstring wFilterName = Str2Wstr(filterName);
	StringCchCopy(wchfilterName, wFilterName.length() + 1, wFilterName.c_str());
	item.pszText = wchfilterName;
	item.mask = LVIF_TEXT | LVIF_STATE;
	item.stateMask = (UINT)-1;
	item.cchTextMax = wFilterName.length() + 1;
	item.iSubItem = 0;
	item.state = 0;// LVIS_FOCUSED | LVIS_SELECTED;
	item.iItem = 0;
	item.cColumns = 1;
	item.lParam = 0;
	int index = SendMessage(hFilterListControl, LVM_INSERTITEM, 0,(LPARAM)&item);
}

void DrvFtaeAlarm::FiltersViewController::ClearFilterView()
{
	EnableWindow(GetDlgItem(window, IDC_EDIT_INSTANCE), TRUE);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_ADDFILTER), TRUE);
	SendDlgItemMessage(window, IDC_EDIT_INSTANCE, EM_SETSEL, 0, -1);
	SendDlgItemMessage(window, IDC_EDIT_INSTANCE, WM_CLEAR, 0, 0);
	SendDlgItemMessage(window, IDC_LIST_CONDITIONS, LVM_DELETEALLITEMS, 0, 0);
	EnableConditionView(FALSE);
}

void DrvFtaeAlarm::FiltersViewController::ActivateFilterView(std::string filterName)
{
	std::wstring wFilterName = Str2Wstr(filterName);
	SendDlgItemMessage(window, IDC_EDIT_INSTANCE, EM_SETSEL, 0, -1);
	SendDlgItemMessage(window, IDC_EDIT_INSTANCE, WM_CLEAR, 0, 0);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_ADDFILTER), FALSE);
	SendDlgItemMessage(window, IDC_EDIT_INSTANCE, WM_SETTEXT, 0, (LPARAM)wFilterName.c_str());
	EnableWindow(GetDlgItem(window, IDC_EDIT_INSTANCE), FALSE);
	EnableConditionView(TRUE);
}

void DrvFtaeAlarm::FiltersViewController::EnableConditionView(BOOL bEnable)
{
	EnableWindow(GetDlgItem(window, IDC_BUTTON_REMOVEFILTER), bEnable);
	EnableWindow(GetDlgItem(window, IDC_LIST_CONDITIONS), bEnable);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_ADDCONDITION), bEnable);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_REMOVECONDITION), bEnable);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_REMOVEALLCONDITIONS), bEnable);
	EnableWindow(GetDlgItem(window, IDC_COMBO_PROPERTY), bEnable);
	EnableWindow(GetDlgItem(window, IDC_COMBO_PROPERTYTYPE), bEnable);
	EnableWindow(GetDlgItem(window, IDC_COMBO_CONDITION), bEnable);
	EnableWindow(GetDlgItem(window, IDC_EDIT_VALUE1), bEnable);
	EnableWindow(GetDlgItem(window, IDC_EDIT_VALUE2), bEnable);
	EnableWindow(GetDlgItem(window, IDC_RADIO_AND), bEnable);
	EnableWindow(GetDlgItem(window, IDC_RADIO_OR), bEnable);
}


void DrvFtaeAlarm::FiltersViewController::SelectFilter(std::string filterName)
{
	ActivateFilterView(filterName);
	presenter->SelectFilter(filterName);
}

INT_PTR WINAPI FiltersDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPNMLISTVIEW itemView;
	memset(&itemView, 0, sizeof(itemView));
	LVITEM itemList;
	memset(&itemList, 0, sizeof(itemList));
	int iItem = 0;
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
			iItem = 0;
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
			controller->AddFilter();
			break;
		case IDC_BUTTON_REMOVEFILTER:
			controller->RemoveFilter();
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
			switch (uNotifyCode)
			{
			case LVN_INSERTITEM:
				itemView = (LPNMLISTVIEW)lParam;
				controller->ClearFilterView();
				break;
			case  LVN_ITEMCHANGED:
				itemView = (LPNMLISTVIEW)lParam;
				iItem = itemView->iItem;
				if ((itemView->uNewState & LVIS_FOCUSED) && (itemView->uNewState & LVIS_SELECTED)) {
					wchar_t wchfilterName[MAX_FILTERNAME_LENGTH];
					itemList.pszText = wchfilterName;
					itemList.cchTextMax = MAX_FILTERNAME_LENGTH;
					size_t nameLength = SendMessage(GetDlgItem(hwnd, IDC_LIST_FILTERS), LVM_GETITEMTEXT, iItem, LPARAM(&itemList));
					wchfilterName[nameLength + 1] = L'\0';
					controller->SelectFilter(Wstr2Str(std::wstring(wchfilterName)));
					/*if(SendDlgItemMessage(hwnd, IDC_LIST_FILTERS, LVM_GETSELECTEDCOUNT, 0, 0) > 1) {
						itemList.state = 0;
						itemList.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
						SendMessage(GetDlgItem(hwnd, IDC_LIST_FILTERS), LVM_SETITEMSTATE, iItem, LPARAM(&itemList));
					}*/
				}
				break;
			case LVN_ITEMCHANGING:
				itemView = (LPNMLISTVIEW)lParam;
				iItem = itemView->iItem;
				if ((itemView->uOldState & LVIS_SELECTED) && (itemView->uNewState == 0)) {
					controller->ClearFilterView();
				}
				break;
			}
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



