#pragma once
#include"SettingsViewController.h"
#include"SettingsPresenter.h"

namespace DrvFtaeAlarm {

	class SettingsInitializer final {
	public:
		static int CreateModule(HINSTANCE hInstance, std::function<ODS::UI::IAbstractUIFacrory* (void)> factoryGetter, HWND* parent, std::shared_ptr<ConnectionAttributes> attributes,
			std::shared_ptr<std::map<std::string, std::vector<StatementCondition> > > filters);
	};
}
