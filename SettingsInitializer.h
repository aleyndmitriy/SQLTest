#pragma once
#include"SettingsViewController.h"
#include"SettingsPresenter.h"

namespace DrvFtaeAlarm {
	class SettingsInitializer final {
	public:
		static void CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent);
	};
}
