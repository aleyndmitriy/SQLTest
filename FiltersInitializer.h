#pragma once

#include"FiltersViewController.h"
#include"FiltersPresenter.h"

namespace DrvFtaeAlarm {
	class FiltersInitializer final {
	public:
		static void CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent, std::shared_ptr<ConnectionAttributes> attributes, std::shared_ptr<std::map<std::string, std::vector<StatementCondition> > > filters);
	};
}
