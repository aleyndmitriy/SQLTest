#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include"resource.h"
#include"Utils.h"
#include"ISettingsViewOutput.h"
#include"UIDialogViewController.h"
#include<vector>

INT_PTR WINAPI SettingDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace DrvFtaeAlarm {

	class SettingsViewController final : public std::enable_shared_from_this<SettingsViewController>,  public UIDialogViewController, public ISettingsViewInput {
	public:
		SettingsViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<ISettingsViewOutput>& output);
		SettingsViewController(SettingsViewController&& rhs) = delete;
		SettingsViewController& operator=(SettingsViewController&& rhs) = delete;
		SettingsViewController(const SettingsViewController& src) = delete;
		SettingsViewController& operator=(const SettingsViewController& rhs) = delete;
		void setupInitialState() override;
		void AddChildViewController(const std::shared_ptr<UIDialogViewController>& child) override;
		void AddToParentView() override;
		void CloseView() override;
		void ShowView() override;
		void HideView() override;
		void OnBtnOkTouched();
		void OnBtnCancelTouched();
		void DestroyLoginView();
		void CreateLoginView();
		void DestroyFiltersView();
		void CreateFiltersView();
		~SettingsViewController();
	private:
		std::shared_ptr<ISettingsViewOutput> presenter;
		std::vector<std::weak_ptr<UIDialogViewController> > childs;
		void ShiftChildDialog();
		bool isOk;
	};

}
