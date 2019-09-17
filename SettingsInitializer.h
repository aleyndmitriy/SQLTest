#pragma once
#include"SettingsViewController.h"
#include"SettingsPresenter.h"

namespace DrvFtaeAlarm {
	class SettingsInitializer final {
	public:
		static int CreateModule(HINSTANCE hInstance, std::function<ODS::UI::IAbstractUIFacrory* (void)> factoryGetter, HWND* parent);
	};
}
