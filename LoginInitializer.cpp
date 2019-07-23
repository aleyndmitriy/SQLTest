#include"LoginInitializer.h"
#include"SQLServerDatabaseEngine.h"

void DrvFtaeAlarm::LoginInitializer::CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent)
{
	std::shared_ptr<ILoginViewOutput> presenter = std::make_shared<LoginPresenter>(std::make_shared<SQLServerDatabaseEngine>());
	LoginViewController* controller = new LoginViewController(parent, presenter);
	HWND hParentHandle = NULL;
	if (parent) {
		HWND* ptrParentHandle = reinterpret_cast<HWND*>(parent->GetInterface());
		hParentHandle = *ptrParentHandle;
	}
	HWND window = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TABPAGE_CONNSETTINGS), hParentHandle, LoginDlg_Proc, (LPARAM)controller);
}