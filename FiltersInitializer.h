#pragma once

#include"FiltersViewController.h"
#include"FiltersPresenter.h"

namespace DrvFtaeAlarm {
	class FiltersInitializer final {
	public:
		static void CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent);
	};
}
