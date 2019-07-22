#pragma once

#include"ISettingsViewOutput.h"

namespace DrvFtaeAlarm {
	class SettingsPresenter final : public std::enable_shared_from_this<SettingsPresenter>, public ISettingsViewOutput
	{
	public:
		SettingsPresenter();
		~SettingsPresenter();
		void setViewInput(std::shared_ptr<ISettingsViewInput> input) override;
		void viewIsReady() override;
	private:
		std::weak_ptr<ISettingsViewInput> view;
	};
}
