#pragma once
#include<Windows.h>
#include<memory>
#include"IExtensible.h"

namespace DrvFtaeAlarm {
	class UIDialogViewController: public IExtensible {
	public:
		UIDialogViewController(const std::shared_ptr<UIDialogViewController> &parent);
		UIDialogViewController(UIDialogViewController&& rhs) = delete;
		UIDialogViewController& operator=(UIDialogViewController&& rhs) = delete;
		UIDialogViewController(const UIDialogViewController& src) = delete;
		UIDialogViewController& operator=(const UIDialogViewController& rhs) = delete;
		virtual ~UIDialogViewController();
		void* GetInterface(int nIfcId) override;
		virtual void AddToParentView() = 0;
		virtual void AddChildViewController(const std::shared_ptr< UIDialogViewController>& child) = 0;
		virtual void CloseView() = 0;
		virtual void ShowView() = 0;
		virtual void HideView() = 0;
	protected:
		std::shared_ptr<UIDialogViewController> parentController;
		HWND window;
	};
}