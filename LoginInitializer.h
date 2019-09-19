#pragma once
#include"LoginViewController.h"
#include"LoginPresenter.h"

namespace DrvFtaeAlarm {
	class LoginInitializer final {
	public:
		static void CreateModule(HINSTANCE hInstance, std::function<ODS::UI::IAbstractUIFacrory* (void)> factoryGetter, const std::shared_ptr<UIDialogViewController>& parent, std::shared_ptr<ConnectionAttributes> attributes);
	};
}