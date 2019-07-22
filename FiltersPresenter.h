#pragma once
#include"IFiltersViewOutput.h"

namespace DrvFtaeAlarm {
	class FiltersPresenter final : public IFiltersViewOutput {
	public:
		FiltersPresenter();
		void viewIsReady() override;
		void SetViewInput(std::shared_ptr<IFiltersViewInput> input) override;
		~FiltersPresenter();
	private:
		std::weak_ptr<IFiltersViewInput> view;
	};
}
