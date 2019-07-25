#pragma once
#include<Windows.h>
#include<CommCtrl.h>
#include"resource.h"
#include"Utils.h"
#include"IFiltersViewOutput.h"
#include"UIDialogViewController.h"

INT_PTR WINAPI FiltersDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace DrvFtaeAlarm {

	class FiltersViewController final : public std::enable_shared_from_this<FiltersViewController>, public UIDialogViewController, public IFiltersViewInput
	{
	public:
		FiltersViewController(const std::shared_ptr<UIDialogViewController>& parent, const std::shared_ptr<IFiltersViewOutput>& output);
		FiltersViewController(FiltersViewController&& rhs) = delete;
		FiltersViewController& operator=(FiltersViewController&& rhs) = delete;
		FiltersViewController(const FiltersViewController& src) = delete;
		FiltersViewController& operator=(const FiltersViewController& rhs) = delete;
		void AddChildViewController(const std::shared_ptr<UIDialogViewController>& child) override;
		void AddToParentView() override;
		void setupInitialState() override;
		void CloseView() override;
		void ShowView() override;
		void HideView() override;
		void LoadConditionsList(const std::vector<StatementCondition>& conditions) override;
		void LoadFiltersList(const std::vector<std::string>& filters) override;
		~FiltersViewController();
	private:
		std::shared_ptr<IFiltersViewOutput> presenter;
		void CreateFiltersList();
		void CreateConditionsList();
	};
}

