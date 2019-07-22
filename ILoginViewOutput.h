#pragma once
#include<memory>
#include"ILoginViewInput.h"

namespace DrvFtaeAlarm {
	class ILoginViewOutput {
	public:
		virtual void viewIsReady() = 0;
		virtual void SetViewInput(std::shared_ptr<ILoginViewInput> input) = 0;
		virtual ~ILoginViewOutput() {};
	};
}