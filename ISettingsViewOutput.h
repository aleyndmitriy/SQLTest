#pragma once
#include<memory>
#include"ISettingsViewInput.h"

namespace DrvFtaeAlarm {
	class ISettingsViewOutput {
	public:
		virtual void viewIsReady() = 0;
		virtual void setViewInput(std::shared_ptr<ISettingsViewInput> input) = 0;
		virtual ~ISettingsViewOutput() {};
	};
}