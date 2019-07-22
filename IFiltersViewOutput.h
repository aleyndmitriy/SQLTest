#pragma once

#include<memory>
#include"IFiltersViewInput.h"

namespace DrvFtaeAlarm {
	class IFiltersViewOutput {
	public:
		virtual void viewIsReady() = 0;
		virtual void SetViewInput(std::shared_ptr<IFiltersViewInput> input) = 0;
		virtual ~IFiltersViewOutput() {};
	};
}