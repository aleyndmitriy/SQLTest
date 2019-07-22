#include"LoginViewController.h"

DrvFtaeAlarm::LoginViewController::LoginViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<ILoginViewOutput>& output): UIDialogViewController(parent),presenter(output)
{

}

void DrvFtaeAlarm::LoginViewController::setupInitialState()
{
	presenter->SetViewInput(shared_from_this());
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


void DrvFtaeAlarm::LoginViewController::VerifyLogin()
{

}

void DrvFtaeAlarm::LoginViewController::VerifyPassword()
{

}

void DrvFtaeAlarm::LoginViewController::SaveLogin()
{

}

void DrvFtaeAlarm::LoginViewController::SavePassword()
{

}

void DrvFtaeAlarm::LoginViewController::ChooseServer()
{

}

void DrvFtaeAlarm::LoginViewController::ConnectToServer()
{

}

void DrvFtaeAlarm::LoginViewController::ChooseAuthentication()
{

}

void DrvFtaeAlarm::LoginViewController::ChooseDatabase()
{

}

void DrvFtaeAlarm::LoginViewController::ConnectToDatabase()
{

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
			case CBN_EDITUPDATE:
				controller->ChooseServer();
				break;
			case EN_CHANGE:
				controller->ConnectToServer();
				break;
			}
			break;
		case IDC_COMBO_AUTH_TYPE:
			switch (HIWORD(wParam))
			{
			case CBN_EDITUPDATE:
				controller->ChooseAuthentication();
				break;
			case EN_CHANGE:
				//controller->ConnectToServer();
				break;
			}
			break;
		case IDC_COMBO_CONFIG_DATABASE_NAME:
			switch (HIWORD(wParam))
			{
			case CBN_EDITUPDATE:
				controller->ChooseDatabase();
				break;
			case EN_CHANGE:
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