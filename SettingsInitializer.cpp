#include"SettingsInitializer.h"
#include"Windows.h"
int DrvFtaeAlarm::SettingsInitializer::CreateModule(HINSTANCE hInstance, std::function<ODS::UI::IAbstractUIFacrory* (void)> factoryGetter,HWND* parent)
{
	std::shared_ptr<ISettingsViewOutput> presenter = std::make_shared<SettingsPresenter>();
	SettingsViewController* controller = new SettingsViewController(std::shared_ptr<DrvFtaeAlarm::UIDialogViewController>(), factoryGetter, presenter);
	HWND hParentHandle = NULL;
	if (parent) {
		hParentHandle = *parent;
	}
	return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_SETTINGS_DIALOG), hParentHandle, SettingDlg_Proc, (LPARAM)controller);
}