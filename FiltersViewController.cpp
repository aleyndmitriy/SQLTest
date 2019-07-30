#include"FiltersViewController.h"
#include"Utils.h"
#include"Constants.h"

//https://stackoverflow.com/questions/26323468/items-and-subitems-in-list-view-control

DrvFtaeAlarm::FiltersViewController::FiltersViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<IFiltersViewOutput>& output) : UIDialogViewController(parent), presenter(output), _properties{}
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
	presenter->viewIsReady();
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
	_properties.clear();
	if (window != NULL) {
		DestroyWindow(window);
		window = 0;
	}
}


void DrvFtaeAlarm::FiltersViewController::LoadPropertiesList(const std::map<std::string, DrvFtaeAlarm::PropertyType>& properties)
{
	_properties = properties;
	CreatePropertiesCombo();
}

void DrvFtaeAlarm::FiltersViewController::CreatePropertiesCombo()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_PROPERTY);
	for(std::map<std::string, PropertyType>::const_iterator itr = _properties.cbegin(); itr != _properties.cend(); ++itr)
	{
		std::wstring wProperty = Str2Wstr(itr->first);
		LRESULT pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)wProperty.c_str());
	}
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
	SendMessage(hFilterListControl, LVM_INSERTCOLUMN, 0, (LPARAM)(&col));
}

void DrvFtaeAlarm::FiltersViewController::CreateConditionsList()
{
	LVCOLUMN col;
	WCHAR wcColumnText[3][10] = { TEXT("Operation"),TEXT("Property"),TEXT("Condition") };
	HWND hConditionListControl = GetDlgItem(window, IDC_LIST_CONDITIONS);
	ListView_SetExtendedListViewStyle(hConditionListControl, LVS_EX_FULLROWSELECT);
	RECT conditionListRect;
	GetWindowRect(hConditionListControl, &conditionListRect);
	for (int i = 0; i < 3; i++)
	{
		memset(&col, 0, sizeof(col));
		col.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
		col.fmt = LVCFMT_LEFT;
		col.pszText = wcColumnText[i];
		if (i == 0) {
			col.cx = (conditionListRect.right - conditionListRect.left) / 10;
		}
		col.cx = 9*(conditionListRect.right - conditionListRect.left)/20; 
		int ind = SendMessage(hConditionListControl, LVM_INSERTCOLUMN, i, (LPARAM)(&col));
	}
}


void DrvFtaeAlarm::FiltersViewController::AddFilter()
{
	WCHAR wchFilterName[MAX_FILTERNAME_LENGTH];
	GetDlgItemText(window, IDC_EDIT_INSTANCE, wchFilterName, MAX_FILTERNAME_LENGTH);
	presenter->AddFilter(Wstr2Str(std::wstring(wchFilterName)));
}

void DrvFtaeAlarm::FiltersViewController::AddCondition()
{
	WCHAR wchFilterName[MAX_FILTERNAME_LENGTH];
	GetDlgItemText(window, IDC_EDIT_INSTANCE, wchFilterName, MAX_FILTERNAME_LENGTH);
	std::string filterName = Wstr2Str(std::wstring(wchFilterName));
	CombineOperation selectedOperation = CombineOperation::COMBINEOP_OR;
	if (IsDlgButtonChecked(window, IDC_RADIO_AND)) {
		selectedOperation = CombineOperation::COMBINEOP_AND;
	}
	std::string selectedProperty = GetSelectedConditionProperty();
	if (selectedProperty.empty() || selectedProperty.length() < 1)
	{
		return;
	}
	PropertyType selectedPropertyType = GetSelectedConditionPropertyType();
	ConditionType selectedCondition = GetSelectedConditionType();
	WCHAR wchValue[STR_LENGTH];
	GetDlgItemText(window, IDC_EDIT_VALUE1, wchValue, STR_LENGTH);
	std::string value1 = Wstr2Str(std::wstring(wchValue));
	GetDlgItemText(window, IDC_EDIT_VALUE2, wchValue, STR_LENGTH);
	std::string value2 = Wstr2Str(std::wstring(wchValue));
	presenter->AddCondition(StatementCondition(selectedOperation, selectedProperty, selectedPropertyType, selectedCondition, value1, value2),filterName);
}


void DrvFtaeAlarm::FiltersViewController::RemoveCondition()
{
	WCHAR wchFilterName[MAX_FILTERNAME_LENGTH];
	GetDlgItemText(window, IDC_EDIT_INSTANCE, wchFilterName, MAX_FILTERNAME_LENGTH);
	std::string filterName = Wstr2Str(std::wstring(wchFilterName));
	LRESULT res = SendDlgItemMessage(window, IDC_LIST_CONDITIONS, LVM_GETNEXTITEM, (WPARAM)-1, (LPARAM)LVNI_FOCUSED);
	if (res > -1)
	{
		presenter->RemoveCondition(res, filterName);
		SendDlgItemMessage(window, IDC_LIST_CONDITIONS, LVM_DELETEITEM, (WPARAM)res, (LPARAM)0);
	}
	
}

void DrvFtaeAlarm::FiltersViewController::RemoveFilter()
{

}

void DrvFtaeAlarm::FiltersViewController::CloseView()
{
	presenter->SaveFilters();
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
	SendMessage(hFilterListControl, LVM_INSERTITEM, 0,(LPARAM)&item);
}

void DrvFtaeAlarm::FiltersViewController::ClearFilterView()
{
	EnableWindow(GetDlgItem(window, IDC_EDIT_INSTANCE), TRUE);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_ADDFILTER), TRUE);
	SendDlgItemMessage(window, IDC_EDIT_INSTANCE, EM_SETSEL, 0, -1);
	SendDlgItemMessage(window, IDC_EDIT_INSTANCE, WM_CLEAR, 0, 0);
	SendDlgItemMessage(window, IDC_LIST_CONDITIONS, LVM_DELETEALLITEMS, 0, 0);
	ClearConditionView();
	EnableConditionView(FALSE);
}

void DrvFtaeAlarm::FiltersViewController::ClearConditionView()
{
	SendDlgItemMessage(window, IDC_COMBO_PROPERTY, CB_SETCURSEL, (WPARAM)-1, 0);
	SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(window, IDC_COMBO_CONDITION, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(window, IDC_EDIT_VALUE1, EM_SETSEL, 0, -1);
	SendDlgItemMessage(window, IDC_EDIT_VALUE1, WM_CLEAR, 0, 0);
	SendDlgItemMessage(window, IDC_EDIT_VALUE2, EM_SETSEL, 0, -1);
	SendDlgItemMessage(window, IDC_EDIT_VALUE2, WM_CLEAR, 0, 0);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_REMOVECONDITION), FALSE);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_REMOVEALLCONDITIONS), FALSE);
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

void DrvFtaeAlarm::FiltersViewController::SelectCondition(int index)
{
	WCHAR wchFilterName[MAX_FILTERNAME_LENGTH];
	GetDlgItemText(window, IDC_EDIT_INSTANCE, wchFilterName, MAX_FILTERNAME_LENGTH);
	std::string filterName = Wstr2Str(std::wstring(wchFilterName));
	presenter->SelectCondition(index, filterName);
}

void DrvFtaeAlarm::FiltersViewController::SelectedCondition(const StatementCondition& condition)
{
	std::wstring wProperty = Str2Wstr(condition.GetProperty());
	LRESULT index = SendDlgItemMessage(window, IDC_COMBO_PROPERTY, CB_FINDSTRING, 0, (LPARAM)wProperty.c_str());
	SendDlgItemMessage(window, IDC_COMBO_PROPERTY, CB_SETCURSEL, (WPARAM)index, 0);
	if (condition.GetCombineOperation() == CombineOperation::COMBINEOP_OR) {
		CheckRadioButton(window, IDC_RADIO_AND, IDC_RADIO_OR, IDC_RADIO_OR);
	}
	else {
		CheckRadioButton(window, IDC_RADIO_AND, IDC_RADIO_OR, IDC_RADIO_AND);
	}
	wProperty = Str2Wstr(PropertyTypeToStr(condition.GetPropertyType()));
	index = SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_ADDSTRING, 0, (LPARAM)wProperty.c_str());
	SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_SETCURSEL, (WPARAM)index, 0);
	wProperty = Str2Wstr(condition.ConditionView());
	index = SendDlgItemMessage(window, IDC_COMBO_CONDITION, CB_ADDSTRING, 0, (LPARAM)wProperty.c_str());
	SendDlgItemMessage(window, IDC_COMBO_CONDITION, CB_SETCURSEL, (WPARAM)index, 0);
	ClearEditValue1Control();
	std::wstring wVal1 = Str2Wstr(condition.GetValue1());
	SendDlgItemMessage(window, IDC_EDIT_VALUE1, WM_SETTEXT, 0, (LPARAM)wVal1.c_str());
	ClearEditValue2Control();
	std::wstring wVal2 = Str2Wstr(condition.GetValue2());
	SendDlgItemMessage(window, IDC_EDIT_VALUE2, WM_SETTEXT, 0, (LPARAM)wVal2.c_str());
	SelectCondition();
}

void DrvFtaeAlarm::FiltersViewController::SelectCondition()
{
	EnableWindow(GetDlgItem(window, IDC_COMBO_PROPERTY), FALSE);
	EnableWindow(GetDlgItem(window, IDC_COMBO_PROPERTYTYPE), FALSE);
	EnableWindow(GetDlgItem(window, IDC_COMBO_CONDITION), FALSE);
	EnableWindow(GetDlgItem(window, IDC_EDIT_VALUE1), FALSE);
	EnableWindow(GetDlgItem(window, IDC_EDIT_VALUE2), FALSE);
	EnableWindow(GetDlgItem(window, IDC_RADIO_AND), FALSE);
	EnableWindow(GetDlgItem(window, IDC_RADIO_OR), FALSE);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_ADDCONDITION), FALSE);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_REMOVECONDITION), TRUE);

}

void DrvFtaeAlarm::FiltersViewController::UnselectCondition()
{
	SendDlgItemMessage(window, IDC_COMBO_PROPERTY, CB_SETCURSEL, (WPARAM)-1, 0);
	EnableWindow(GetDlgItem(window, IDC_COMBO_PROPERTY), TRUE);
	EnableWindow(GetDlgItem(window, IDC_COMBO_PROPERTYTYPE), TRUE);
	EnableWindow(GetDlgItem(window, IDC_COMBO_CONDITION), TRUE);
	EnableWindow(GetDlgItem(window, IDC_EDIT_VALUE1), TRUE);
	EnableWindow(GetDlgItem(window, IDC_EDIT_VALUE2), TRUE);
	EnableWindow(GetDlgItem(window, IDC_RADIO_AND), TRUE);
	EnableWindow(GetDlgItem(window, IDC_RADIO_OR), TRUE);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_ADDCONDITION), TRUE);
	EnableWindow(GetDlgItem(window, IDC_BUTTON_REMOVECONDITION), FALSE);
	SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(window, IDC_COMBO_CONDITION, CB_RESETCONTENT, 0, 0);
	ClearEditValue1Control();
	ClearEditValue2Control();
}

void DrvFtaeAlarm::FiltersViewController::AddCondition(const StatementCondition& condition, int index)
{
	HWND hConditionListControl = GetDlgItem(window, IDC_LIST_CONDITIONS);
	LVITEM item;
	wchar_t wchConditionOperation[4];
	wchar_t wchConditionProperty[STR_LENGTH];
	wchar_t wchConditionType[STR_LENGTH];
	switch (condition.GetCombineOperation())
	{
	case CombineOperation::COMBINEOP_OR:
		StringCchCopy(wchConditionOperation, 3, TEXT("OR"));
		break;
	case CombineOperation::COMBINEOP_AND:
		StringCchCopy(wchConditionOperation, 4, TEXT("AND"));
		break;
	default:
		break;
	}
	item.pszText = wchConditionOperation;
	item.mask = LVIF_TEXT | LVIF_STATE;
	item.stateMask = (UINT)-1;
	item.cchTextMax = STR_LENGTH;
	item.iSubItem = 0;
	item.state = 0;// LVIS_FOCUSED | LVIS_SELECTED;
	item.iItem = index;
	item.cColumns = 3;
	item.lParam = 0;
	SendMessage(hConditionListControl, LVM_INSERTITEM, 0, (LPARAM)& item);
	std::wstring wConditionView = Str2Wstr(condition.PropertyView());
	StringCchCopy(wchConditionProperty, STR_LENGTH, wConditionView.c_str());
	item.iSubItem = 1;
	item.pszText = wchConditionProperty;
	SendMessage(hConditionListControl, LVM_SETITEM, 0, (LPARAM)& item);
	std::wstring wConditionType = Str2Wstr(condition.ConditionView());
	StringCchCopy(wchConditionType, STR_LENGTH, wConditionType.c_str());
	item.iSubItem = 2;
	item.pszText = wchConditionType;
	SendMessage(hConditionListControl, LVM_SETITEM, 0, (LPARAM)& item);
}

std::string DrvFtaeAlarm::FiltersViewController::GetSelectedConditionProperty()
{
	wchar_t wchPropertyName[MAX_COLUMNS];
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_PROPERTY);
	LRESULT index = SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	LRESULT res = SendMessage(hComboControl, CB_GETLBTEXT, index, (LPARAM)wchPropertyName);
	if (res < 1) {
		return std::string();
	}
	wchPropertyName[res + 1] = L'\0';
	std::wstring wStr = std::wstring(wchPropertyName);
	std::string propertyName = Wstr2Str(wStr);
	return propertyName;
}


DrvFtaeAlarm::PropertyType DrvFtaeAlarm::FiltersViewController::GetSelectedConditionPropertyType()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_PROPERTYTYPE);
	LRESULT index = SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	LRESULT res = SendMessage(hComboControl, CB_GETITEMDATA, (WPARAM)index, 0);
	PropertyType type = PropertyType::PROPTYPE_NONE;
	switch (res)
	{
	case 0:
		type = PropertyType::PROPTYPE_NUMERIC;
			break;
	case 1:
		type = PropertyType::PROPTYPE_TEXT;
		break;
	case 2:
		type = PropertyType::PROPTYPE_BOOLEAN;
		break;
	case 3:
		type = PropertyType::PROPTYPE_DATE;
		break;
	default:
		break;
	}
	return type;
}

DrvFtaeAlarm::ConditionType DrvFtaeAlarm::FiltersViewController::GetSelectedConditionType()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONDITION);
	LRESULT index = SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	LRESULT res = SendMessage(hComboControl, CB_GETITEMDATA, (WPARAM)index, 0);
	ConditionType type = ConditionType::CONDTYPE_NONE;
	switch (res)
	{
	case 0:
		type = ConditionType::CONDTYPE_EQUAL;
		break;
	case 1:
		type = ConditionType::CONDTYPE_LESS;
		break;
	case 2:
		type = ConditionType::CONDTYPE_GREATER;
		break;
	case 3:
		type = ConditionType::CONDTYPE_LESSEQUAL;
		break;
	case 4:
		type = ConditionType::CONDTYPE_GREATEREQUAL;
		break;
	case 5:
		type = ConditionType::CONDTYPE_NOTEQUAL;
		break;
	case 6:
		type = ConditionType::CONDTYPE_BETWEEN;
		break;
	case 7:
		type = ConditionType::CONDTYPE_ISNULL;
		break;
	case 8:
		type = ConditionType::CONDTYPE_ISNOTNULL;
		break;
	case 9:
		type = ConditionType::CONDTYPE_LIKE;
		break;
	default:
		break;
	}
	return type;
}


void DrvFtaeAlarm::FiltersViewController::ChooseConditionProperty()
{
	std::string propertyName = GetSelectedConditionProperty();
	std::map<std::string, PropertyType>::const_iterator itr = _properties.find(propertyName);
	if (itr != _properties.cend()) {
		PropertyType oldType = GetSelectedConditionPropertyType();
		if (oldType != itr->second) {
			CreatePropertyTypeCombo(itr->second);
			SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_SETCURSEL, (WPARAM)0, 0);
			SendDlgItemMessage(window, IDC_COMBO_CONDITION, CB_RESETCONTENT, 0, 0);
			CreateConditionCombo(itr->second);
		}
	}
}

void DrvFtaeAlarm::FiltersViewController::CreatePropertyTypeCombo(PropertyType propertyType)
{
	SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_RESETCONTENT, 0, 0);
	LRESULT pos = 0;
	wchar_t wchConditionProperty[STR_LENGTH];
	switch (propertyType)
	{
	case PropertyType::PROPTYPE_NUMERIC:
		StringCchCopy(wchConditionProperty, STR_LENGTH, TEXT("NUMERIC"));
		pos = SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_ADDSTRING, 0, LPARAM(wchConditionProperty));
		SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_SETITEMDATA, pos, (LPARAM)0);
		break;
	case PropertyType::PROPTYPE_TEXT:
		StringCchCopy(wchConditionProperty, STR_LENGTH, TEXT("TEXT"));
		pos = SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_ADDSTRING, 0, LPARAM(wchConditionProperty));
		SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_SETITEMDATA, pos, (LPARAM)1);
		break;
	case PropertyType::PROPTYPE_BOOLEAN:
		StringCchCopy(wchConditionProperty, STR_LENGTH, TEXT("BOOLEAN"));
		pos = SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_ADDSTRING, 0, LPARAM(wchConditionProperty));
		SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_SETITEMDATA, pos, (LPARAM)2);
		break;
	case PropertyType::PROPTYPE_DATE:
		StringCchCopy(wchConditionProperty, STR_LENGTH, TEXT("DATE"));
		pos = SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_ADDSTRING, 0, LPARAM(wchConditionProperty));
		SendDlgItemMessage(window, IDC_COMBO_PROPERTYTYPE, CB_SETITEMDATA, pos, (LPARAM)3);
		break;
	}
	
}

void DrvFtaeAlarm::FiltersViewController::ChooseConditionPropertyType()
{
	PropertyType type = GetSelectedConditionPropertyType();
	CreateConditionCombo(type);
}

void DrvFtaeAlarm::FiltersViewController::ChooseCondition()
{
	ClearEditValue1Control();
	ClearEditValue2Control();
	ShowEditValuesControls();
}

void DrvFtaeAlarm::FiltersViewController::ShowEditValuesControls()
{
	HWND hEditControl1 = GetDlgItem(window, IDC_EDIT_VALUE1);
	HWND hEditControl2 = GetDlgItem(window, IDC_EDIT_VALUE2);
	ConditionType type = GetSelectedConditionType();
	switch (type)
	{
	case ConditionType::CONDTYPE_EQUAL:
	case ConditionType::CONDTYPE_LESS:
	case ConditionType::CONDTYPE_GREATER:
	case ConditionType::CONDTYPE_LESSEQUAL:
	case ConditionType::CONDTYPE_GREATEREQUAL:
	case ConditionType::CONDTYPE_NOTEQUAL:
	case ConditionType::CONDTYPE_LIKE:
		ShowWindow(hEditControl1, TRUE);
		ShowWindow(hEditControl2, FALSE);
		break;
	case ConditionType::CONDTYPE_BETWEEN:
		ShowWindow(hEditControl1, TRUE);
		ShowWindow(hEditControl2, TRUE);
		break;
	case ConditionType::CONDTYPE_ISNULL:
	case ConditionType::CONDTYPE_ISNOTNULL:
		ShowWindow(hEditControl1, FALSE);
		ShowWindow(hEditControl2, FALSE);
		break;
	default:
		break;
	}
}

void DrvFtaeAlarm::FiltersViewController::ClearEditValue1Control()
{
	HWND hEditControl = GetDlgItem(window, IDC_EDIT_VALUE1);
	SendMessage(hEditControl, EM_SETSEL, 0, -1);
	SendMessage(hEditControl, WM_CLEAR, 0, 0);
}

void DrvFtaeAlarm::FiltersViewController::ClearEditValue2Control()
{
	HWND hEditControl = GetDlgItem(window, IDC_EDIT_VALUE2);
	SendMessage(hEditControl, EM_SETSEL, 0, -1);
	SendMessage(hEditControl, WM_CLEAR, 0, 0);
}

void DrvFtaeAlarm::FiltersViewController::CreateConditionCombo(PropertyType propertyType)
{
	LRESULT pos = 0;
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONDITION);
	SendMessage(hComboControl, CB_RESETCONTENT, 0, 0);
	switch (propertyType)
	{
	case PropertyType::PROPTYPE_NUMERIC:
	case PropertyType::PROPTYPE_DATE:
		CreateConditionComboNumeric();
		break;
	case PropertyType::PROPTYPE_TEXT:
		CreateConditionComboNumeric();
		pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("LIKE"));
		SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_LIKE);
		break;
	case PropertyType::PROPTYPE_BOOLEAN:
		CreateConditionComboBoolean();
			break;
		default:
			break;
	}
}


void DrvFtaeAlarm::FiltersViewController::CreateConditionComboNumeric()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONDITION);
	LRESULT pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("EQUAL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_EQUAL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("LESS"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_LESS);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("GREATER"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_GREATER);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("LESS OR EQUAL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_LESSEQUAL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("GREATER OR EQUAL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_GREATEREQUAL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("NOT EQUAL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_NOTEQUAL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("BETWEEN"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_BETWEEN);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("IS NULL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_ISNULL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("IS NOT NULL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_ISNOTNULL);
}

void DrvFtaeAlarm::FiltersViewController::CreateConditionComboBoolean()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONDITION);
	LRESULT pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("TRUE"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_EQUAL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("FALSE"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_NOTEQUAL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("IS NULL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_ISNULL);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("IS NOT NULL"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)ConditionType::CONDTYPE_ISNOTNULL);
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
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				controller->ChooseConditionProperty();
				break;
			}
			break;
		case IDC_COMBO_PROPERTYTYPE:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				controller->ChooseConditionPropertyType();
				break;
			}
			break;
		case IDC_COMBO_CONDITION:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				controller->ChooseCondition();
				break;
			}
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
			controller->AddCondition();
			break;
		case IDC_BUTTON_REMOVECONDITION:
			controller->RemoveCondition();
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
			case LVN_INSERTITEM:
				controller->ClearConditionView();
				break;
			case  LVN_ITEMCHANGED:
				itemView = (LPNMLISTVIEW)lParam;
				iItem = itemView->iItem;
				if ((itemView->uNewState & LVIS_FOCUSED) && (itemView->uNewState & LVIS_SELECTED)) {
					controller->SelectCondition(iItem);
				}
				break;
			case LVN_ITEMCHANGING:
				itemView = (LPNMLISTVIEW)lParam;
				iItem = itemView->iItem;
				if ((itemView->uOldState & LVIS_SELECTED) && (itemView->uNewState == 0)) {
					controller->UnselectCondition();
				}
				break;
			case LVN_DELETEALLITEMS:
				break;
			case LVN_DELETEITEM:
				
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



