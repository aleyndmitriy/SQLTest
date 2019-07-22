#pragma once
#include"LoginViewController.h"
#include"LoginPresenter.h"

namespace DrvFtaeAlarm {
	class LoginInitializer final {
	public:
		static void CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent);
	};
}