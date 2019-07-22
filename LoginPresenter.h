#pragma once
#include"ILoginViewOutput.h"

namespace DrvFtaeAlarm {
	class LoginPresenter final: public ILoginViewOutput {
	public:
		LoginPresenter();
		void viewIsReady() override;
		void SetViewInput(std::shared_ptr<ILoginViewInput> input) override;
		 ~LoginPresenter();
	private:
		std::weak_ptr<ILoginViewInput> view;
	};
}