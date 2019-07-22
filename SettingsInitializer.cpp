#include"SettingsInitializer.h"
#include"Windows.h"
void DrvFtaeAlarm::SettingsInitializer::CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent)
{
	std::shared_ptr<ISettingsViewOutput> presenter = std::make_shared<SettingsPresenter>();
	SettingsViewController* controller = new SettingsViewController(parent, presenter);
	HWND hParenHandle = NULL;
	if (parent) {
		hParenHandle = (HWND)(parent->GetInterface());
	}
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_SETTINGS_DIALOG), hParenHandle, SettingDlg_Proc, (LPARAM)controller);
}