#include"LoginViewController.h"
#include"Constants.h"
#include"Utils.h"

DrvFtaeAlarm::LoginViewController::LoginViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<ILoginViewOutput>& output): UIDialogViewController(parent),presenter(output)
{

}

void DrvFtaeAlarm::LoginViewController::setupInitialState()
{
	presenter->SetViewInput(shared_from_this());
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
	presenter->FindServerList();
}
void DrvFtaeAlarm::LoginViewController::VerifyLogin()
{

}

void DrvFtaeAlarm::LoginViewController::VerifyPassword()
{

}

void DrvFtaeAlarm::LoginViewController::SaveLogin()
{
	wchar_t login[STR_LENGTH];
	GetDlgItemText(window, IDC_EDIT_USERNAME, login, STR_LENGTH);
	std::wstring wStr(login);
	presenter->GetLogin(Wstr2Str(wStr));
}

void DrvFtaeAlarm::LoginViewController::SavePassword()
{
	wchar_t password[STR_LENGTH];
	GetDlgItemText(window, IDC_EDIT_PASSWORD, password, STR_LENGTH);
	std::wstring wStr(password);
	presenter->GetPassword(Wstr2Str(wStr));
}

void DrvFtaeAlarm::LoginViewController::ChooseServer()
{
	wchar_t server[STR_LENGTH];
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_SERVER_NAME);
	int index = (int)SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	int res = SendMessage(hComboControl, CB_GETITEMDATA, index, 0);
	presenter->GetServerIndex(res);
}

void DrvFtaeAlarm::LoginViewController::ConnectToServer()
{
	presenter->ConnectToServer();
}

void DrvFtaeAlarm::LoginViewController::ChooseAuthentication()
{

}

void DrvFtaeAlarm::LoginViewController::ChooseDatabase()
{
	wchar_t server[STR_LENGTH];
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONFIG_DATABASE_NAME);
	int index = (int)SendMessage(hComboControl, CB_GETCURSEL, 0, 0);
	int res = SendMessage(hComboControl, CB_GETITEMDATA, index, 0);
	presenter->GetDatabaseIndex(res);
}

void DrvFtaeAlarm::LoginViewController::ConnectToDatabase()
{
	presenter->ConnectToDatabase();
}

void DrvFtaeAlarm::LoginViewController::CloseView()
{
	SendMessage(window, WM_CLOSE, 0, 0);
}

void DrvFtaeAlarm::LoginViewController::ShowView()
{
	ShowWindow(window, SW_SHOW);
	
}

void DrvFtaeAlarm::LoginViewController::HideView() {
	ShowWindow(window, SW_HIDE);
}

void DrvFtaeAlarm::LoginViewController::LoadServerList(const std::vector<std::string>& servers)
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_SERVER_NAME);
	SendMessage(hComboControl, CB_RESETCONTENT, 0, 0);
	size_t index = 0;
	for (std::vector<std::string>::const_iterator itr = servers.cbegin(); itr != servers.cend(); ++itr)
	{
		std::wstring str = Str2Wstr(*itr);
		int pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)str.c_str());
		SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)index++);
	}
}

void DrvFtaeAlarm::LoginViewController::LoadDatabasesList(const std::vector<std::string>& databases)
{
	HWND hComboControl = GetDlgItem(window, IDC_COMBO_CONFIG_DATABASE_NAME);
	SendMessage(hComboControl, CB_RESETCONTENT, 0, 0);
	size_t index = 0;
	for (std::vector<std::string>::const_iterator itr = databases.cbegin(); itr != databases.cend(); ++itr)
	{
		std::wstring str = Str2Wstr(*itr);
		int pos = SendMessage(hComboControl, CB_ADDSTRING, 0, (LPARAM)str.c_str());
		SendMessage(hComboControl, CB_SETITEMDATA, pos, (LPARAM)index++);
	}
}


void DrvFtaeAlarm::LoginViewController::SaveServerName()
{
	wchar_t server[STR_LENGTH];
	GetDlgItemText(window, IDC_COMBO_SERVER_NAME, server, STR_LENGTH);
	std::wstring wStr(server);
	presenter->GetServerName(Wstr2Str(wStr));
}

void DrvFtaeAlarm::LoginViewController::SaveDatabaseName()
{
	wchar_t base[STR_LENGTH];
	GetDlgItemText(window, IDC_COMBO_CONFIG_DATABASE_NAME, base, STR_LENGTH);
	std::wstring wStr(base);
	presenter->GetDatabaseName(Wstr2Str(wStr));
}


INT_PTR WINAPI LoginDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iTab = 0;
	HWND* ptrHandle = NULL;
	static std::shared_ptr<DrvFtaeAlarm::LoginViewController> controller;
	switch (uMsg) {
	case WM_INITDIALOG:
		controller = std::shared_ptr<DrvFtaeAlarm::LoginViewController>(reinterpret_cast<DrvFtaeAlarm::LoginViewController*>(lParam));
		ptrHandle = reinterpret_cast<HWND*>(controller->GetInterface());
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
				controller->LoadSeverInstances();
				break;
			case CBN_EDITUPDATE:
				//controller->ChooseServer();
				break;
			case CBN_EDITCHANGE:
				controller->SaveServerName();
				break;
			case CBN_SELCHANGE:
				controller->ChooseServer();
				break;
			case CBN_SELENDOK:
				controller->ChooseServer();
				break;
			}
			break;
		case IDC_COMBO_AUTH_TYPE:
			switch (HIWORD(wParam))
			{
			case CBN_EDITUPDATE:
				controller->ChooseAuthentication();
				break;
			case CBN_SELCHANGE:
				//controller->ConnectToServer();
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
				controller->ConnectToDatabase();
				break;
			}
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