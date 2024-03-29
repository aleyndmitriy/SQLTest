#include"LoginViewController.h"
#include"Constants.h"
#include"Utils.h"

DrvFtaeAlarm::LoginViewController::LoginViewController(const std::shared_ptr<UIDialogViewController>& parent, std::function<ODS::UI::IAbstractUIFacrory* (void)> factoryGetter, const std::shared_ptr<ILoginViewOutput>& output): UIDialogViewController(parent),presenter(output), uiFactoryGetter(factoryGetter), sqlBrowser(nullptr)
{

}

void DrvFtaeAlarm::LoginViewController::setupInitialState()
{
	presenter->SetViewInput(shared_from_this());
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_AUTH_TYPE);
	SendMessage(hComboControl, CB_RESETCONTENT, 0, 0);
	int pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("Windows Authentication"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)0);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("SQL Server Authentication"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)1);
	SendMessage(hComboControl, CB_SETCURSEL, (WPARAM)0, 0);
	HWND hloginControl = GetDlgItem(window, IDC_EDIT_USERNAME);
	HWND hpassControl = GetDlgItem(window, IDC_EDIT_PASSWORD);
	EnableWindow(hloginControl, FALSE);
	EnableWindow(hpassControl, FALSE);
	presenter->viewIsReady();
}

DrvFtaeAlarm::LoginViewController::~LoginViewController()
{
	presenter.reset();
	if (window != NULL) {
		DestroyWindow(window);
		window = NULL;
	}
}

void DrvFtaeAlarm::LoginViewController::AddChildViewController(const std::shared_ptr<UIDialogViewController>& child)
{

}

void DrvFtaeAlarm::LoginViewController::AddToParentView()
{
	parentController->AddChildViewController(shared_from_this());
}

void DrvFtaeAlarm::LoginViewController::LoadSeverInstances()
{
	presenter->ConnectToDriver();
}
void DrvFtaeAlarm::LoginViewController::VerifyLogin()
{

}

void DrvFtaeAlarm::LoginViewController::VerifyPassword()
{

}

void DrvFtaeAlarm::LoginViewController::SaveLogin()
{
	TCHAR login[STR_LENGTH];
	GetDlgItemText(window, IDC_EDIT_USERNAME, login, STR_LENGTH);
	presenter->GetLogin(std::string(login));
}

void DrvFtaeAlarm::LoginViewController::SavePassword()
{
	TCHAR password[STR_LENGTH];
	GetDlgItemText(window, IDC_EDIT_PASSWORD, password, STR_LENGTH);
	presenter->GetPassword(std::string(password));
}

void DrvFtaeAlarm::LoginViewController::ChooseServer()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_SERVER_NAME);
	int index = (int)SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	if (index) {
		SendDlgItemMessage(window, IDC_COMBO_AUTH_TYPE, CB_SETCURSEL, (WPARAM)0, 0);
		HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONFIG_DATABASE_NAME);
		SendMessage(hComboControl, CB_RESETCONTENT, 0, 0);
		HWND hloginControl = GetDlgItem(window, IDC_EDIT_USERNAME);
		SendMessage(hloginControl, EM_SETSEL, 0, -1);
		SendMessage(hloginControl, WM_CLEAR, 0, 0);
		EnableWindow(hloginControl, FALSE);
		HWND hpassControl = GetDlgItem(window, IDC_EDIT_PASSWORD);
		SendMessage(hpassControl, EM_SETSEL, 0, -1);
		SendMessage(hpassControl, WM_CLEAR, 0, 0);
		EnableWindow(hpassControl, FALSE);
		ODS::OdsString server;
		sqlBrowser.reset(dynamic_cast<ODS::Resources::ISqlBrowser*>(uiFactoryGetter()->CreateSqlBrowser()));
		sqlBrowser->OpenSourcesBrowserDlg(window, "Browse SQL Servers", server);
		if (server.IsEmpty()) {
			SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_SETCURSEL, (WPARAM)0, 0);
		}
		else {
			LRESULT ind = SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_FINDSTRING, 0, (LPARAM)server.GetString());
			if (ind == CB_ERR) {
				ind = SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_ADDSTRING, 0, (LPARAM)server.GetString());
				SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_SETCURSEL, (WPARAM)ind, 0);
			}
			else {
				SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_SETCURSEL, (WPARAM)ind, 0);
			}
		}
	}
	else {
		SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_SETCURSEL, (WPARAM)0, 0);
	}
	SaveServerName();
	sqlBrowser.reset();
}

void DrvFtaeAlarm::LoginViewController::ConnectToServer()
{
	presenter->ConnectToServer();
}

void DrvFtaeAlarm::LoginViewController::ChooseAuthentication()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_AUTH_TYPE);
	int index = (int)SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	int res = SendMessage(hComboControl, CB_GETITEMDATA, index, 0);
	HWND hloginControl = GetDlgItem(window, IDC_EDIT_USERNAME);
	HWND hpassControl = GetDlgItem(window, IDC_EDIT_PASSWORD);
	if (!res) {
		SendMessage(hloginControl, EM_SETSEL, 0, -1);
		SendMessage(hloginControl, WM_CLEAR, 0, 0);
		SendMessage(hpassControl, EM_SETSEL, 0, -1);
		SendMessage(hpassControl, WM_CLEAR, 0, 0);
		EnableWindow(hloginControl, FALSE);
		EnableWindow(hpassControl, FALSE);
	}
	else {
		EnableWindow(hloginControl, TRUE);
		EnableWindow(hpassControl, TRUE);
	}
	presenter->GetAuthType(res);
}

void DrvFtaeAlarm::LoginViewController::ChooseDatabase()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONFIG_DATABASE_NAME);
	LRESULT index = (int)SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	LRESULT res = SendMessage(hComboControl, CB_GETITEMDATA, (WPARAM)index, 0);
	presenter->GetDatabaseIndex(res);
}

void DrvFtaeAlarm::LoginViewController::CheckConnectionToDatabase()
{
	presenter->CheckConnectToDatabase();
}

void DrvFtaeAlarm::LoginViewController::CloseView()
{
	presenter->SaveSettings();
	SendMessage(window, WM_CLOSE, 0, 0);
}

void DrvFtaeAlarm::LoginViewController::ShowView()
{
	ShowWindow(window, SW_SHOW);
	
}

void DrvFtaeAlarm::LoginViewController::HideView() {

	presenter->SaveSettings();
	ShowWindow(window, SW_HIDE);
}

void DrvFtaeAlarm::LoginViewController::ChooseLocalOrRemoteServers()
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_SERVER_NAME);
	SendMessage(hComboControl, CB_RESETCONTENT, 0, 0);
	int pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("(local)"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)0);
	pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)TEXT("<Browse for more...>"));
	SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)1);
}

void DrvFtaeAlarm::LoginViewController::LoadDatabasesList(const std::vector<std::string>& databases)
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONFIG_DATABASE_NAME);
	SendMessage(hComboControl, CB_RESETCONTENT, 0, 0);
	size_t index = 0;
	for (std::vector<std::string>::const_iterator itr = databases.cbegin(); itr != databases.cend(); ++itr)
	{
		int pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)itr->c_str());
		SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)index++);
	}
}

void DrvFtaeAlarm::LoginViewController::SelectDatabase(std::string databaseName)
{
	LRESULT index = SendDlgItemMessage(window, IDC_COMBO_CONFIG_DATABASE_NAME, CB_FINDSTRING, 0, (LPARAM)databaseName.c_str());
	if (index == CB_ERR) {
		LRESULT pos = SendDlgItemMessage(window, IDC_COMBO_CONFIG_DATABASE_NAME, CB_ADDSTRING, 0, (LPARAM)databaseName.c_str());
		SendDlgItemMessage(window, IDC_COMBO_CONFIG_DATABASE_NAME, CB_SETCURSEL, (WPARAM)pos, 0);
	}
	else {
		SendDlgItemMessage(window, IDC_COMBO_CONFIG_DATABASE_NAME, CB_SETCURSEL, (WPARAM)index, 0);
	}
}

void DrvFtaeAlarm::LoginViewController::LoadConnectionSettings(const ConnectionAttributes& attributes)
{
	
	if (!attributes.serverName.empty()) {
		LRESULT index = SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_ADDSTRING, 0, (LPARAM)(attributes.serverName.c_str()));
		SendDlgItemMessage(window, IDC_COMBO_SERVER_NAME, CB_SETCURSEL, (WPARAM)index, 0);
	}
	if (!attributes.databaseName.empty()) {
		LRESULT index = SendDlgItemMessage(window, IDC_COMBO_CONFIG_DATABASE_NAME, CB_ADDSTRING, 0, (LPARAM)(attributes.databaseName.c_str()));
		SendDlgItemMessage(window, IDC_COMBO_CONFIG_DATABASE_NAME, CB_SETCURSEL, (WPARAM)index, 0);
	}
	
	HWND hloginControl = GetDlgItem(window, IDC_EDIT_USERNAME);
	SendMessage(hloginControl, EM_SETSEL, 0, -1);
	SendMessage(hloginControl, WM_CLEAR, 0, 0);
	HWND hpassControl = GetDlgItem(window, IDC_EDIT_PASSWORD);
	SendMessage(hpassControl, EM_SETSEL, 0, -1);
	SendMessage(hpassControl, WM_CLEAR, 0, 0);
	if (attributes.isServerAuthentication) {
		SendDlgItemMessage(window, IDC_COMBO_AUTH_TYPE, CB_SETCURSEL, (WPARAM)1, 0);
		EnableWindow(hloginControl, TRUE);
		EnableWindow(hpassControl, TRUE);
		if (!attributes.loginName.empty()) {
			SendMessage(hloginControl, WM_SETTEXT, 0, (LPARAM)(attributes.loginName.c_str()));
		}
		if (!attributes.password.empty()) {
			SendMessage(hpassControl, WM_SETTEXT, 0, (LPARAM)(attributes.password.c_str()));
		}
	}
	else {
		SendDlgItemMessage(window, IDC_COMBO_AUTH_TYPE, CB_SETCURSEL, (WPARAM)0, 0);
		EnableWindow(hloginControl,FALSE);
		EnableWindow(hpassControl, FALSE);
	}
	if (attributes.isAlarmReport) {
		CheckRadioButton(window, IDC_RADIO_ALARM, IDC_RADIO_EVENT, IDC_RADIO_ALARM);
	}
	else {
		CheckRadioButton(window, IDC_RADIO_ALARM, IDC_RADIO_EVENT, IDC_RADIO_EVENT);
	}
}

void DrvFtaeAlarm::LoginViewController::SaveServerName()
{
	TCHAR server[STR_LENGTH];
	GetDlgItemText(window, IDC_COMBO_SERVER_NAME, server, STR_LENGTH);
	presenter->GetServerName(std::string(server));
}

void DrvFtaeAlarm::LoginViewController::SaveDatabaseName()
{
	TCHAR base[STR_LENGTH];
	GetDlgItemText(window, IDC_COMBO_CONFIG_DATABASE_NAME, base, STR_LENGTH);
	presenter->GetDatabaseName(std::string(base));
}

void DrvFtaeAlarm::LoginViewController::SetAlarmReportType()
{
	presenter->GetReportType(true);
}

void  DrvFtaeAlarm::LoginViewController::SetEventReportType()
{
	presenter->GetReportType(false);
}

void DrvFtaeAlarm::LoginViewController::StartLoading()
{
	HCURSOR hCurs = LoadCursor(NULL, IDC_WAIT);
	SetCursor(hCurs);
}
void DrvFtaeAlarm::LoginViewController::StopLoading()
{
	HCURSOR hCurs = LoadCursor(NULL, IDC_ARROW);
	SetCursor(hCurs);
}

void DrvFtaeAlarm::LoginViewController::WarningMessage(std::string message)
{
	MessageBox(window,TEXT(message.c_str()),"Warning", MB_ICONWARNING);
}

void DrvFtaeAlarm::LoginViewController::ErrorMessage(std::string message)
{
	MessageBox(window,TEXT(message.c_str()), "Warning", MB_ICONSTOP);
}

INT_PTR WINAPI LoginDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND* ptrHandle = NULL;
	static std::shared_ptr<DrvFtaeAlarm::LoginViewController> controller;
	
	switch (uMsg) {
	case WM_INITDIALOG:
		controller = std::shared_ptr<DrvFtaeAlarm::LoginViewController>(reinterpret_cast<DrvFtaeAlarm::LoginViewController*>(lParam));
		ptrHandle = reinterpret_cast<HWND*>(controller->GetInterface(0));
		*ptrHandle = hwnd;
		controller->setupInitialState();
		controller->AddToParentView();
		break;
	case WM_CLOSE:
		controller.reset();
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_USERNAME: 
			switch (HIWORD(wParam))
			{
			case EN_UPDATE:
				controller->VerifyLogin();
				break;
			case EN_CHANGE:
				controller->SaveLogin();
				break;
			}
			break;
		case IDC_EDIT_PASSWORD:
			switch (HIWORD(wParam))
			{
			case EN_UPDATE:
				controller->VerifyPassword();
				break;
			case EN_CHANGE:
				controller->SavePassword();
				break;
			}
			break;
		case IDC_COMBO_SERVER_NAME:
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				controller->ChooseLocalOrRemoteServers();
				break;
			case CBN_EDITCHANGE:
				controller->SaveServerName();
				break;
			case CBN_SELENDOK:
				controller->ChooseServer();
				break;
			}
			break;
		case IDC_COMBO_AUTH_TYPE:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				controller->ChooseAuthentication();
				break;
			}
			break;
		case IDC_COMBO_CONFIG_DATABASE_NAME:
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
				controller->ConnectToServer();
				break;
			case CBN_EDITUPDATE:
				controller->SaveDatabaseName();
				break;
			case CBN_EDITCHANGE:
				controller->SaveDatabaseName();
				break;
			case CBN_SELCHANGE:
				controller->ChooseDatabase();
				break;
			case CBN_SELENDOK:
				controller->ChooseDatabase();
				break;
			}
			break;
		case IDC_BUTTON_TESTCONNECTION:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				controller->CheckConnectionToDatabase();
				break;
			}
			break;
		case IDC_RADIO_ALARM:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				controller->SetAlarmReportType();
				break;
			}
			break;
		case IDC_RADIO_EVENT:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				controller->SetEventReportType();
				break;
			}
			break;
		}
		break;
	case WM_DESTROY:
		//PostQuitMessage(0);
		break;
	}

	return(FALSE);
}