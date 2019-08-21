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
		void LoadPropertiesList(const std::map<std::string, PropertyType>& properties) override;
		void AddFilter();
		void RemoveFilter();
		void WarningMessage(std::string message) override;
		void AddFilter(std::string filterName) override;
		void SelectFilter(std::string filterName);
		void SelectCondition(int index);
		void ClearFilterView();
		void ClearConditionView();
		void SelectCondition();
		void UnselectCondition();
		void AddCondition(const StatementCondition& condition, int index) override;
		void SelectedCondition(const StatementCondition& condition) override;
		void AddCondition();
		void RemoveCondition();
		void RemoveAllConditions();
		void ChooseConditionProperty();
		void ChooseConditionPropertyType();
		void ChooseCondition();
		void LoadColumnsName();
		void StartLoading() override;
		void StopLoading() override;
		~FiltersViewController();
	private:
		std::shared_ptr<IFiltersViewOutput> presenter;
		std::map<std::string, PropertyType> _properties;
		void CreateFiltersList();
		void CreateConditionsList();
		void ActivateFilterView(std::string filterName);
		void EnableConditionView(BOOL bEnable);
		void CreatePropertiesCombo();
		void CreateConditionCombo(PropertyType propertyType);
		void CreateConditionComboNumeric();
		void CreateConditionComboBoolean();
		void CreatePropertyTypeCombo(PropertyType propertyType);
		void ShowEditValuesControls();
		void ClearEditValue1Control();
		void ClearEditValue2Control();
		std::string GetSelectedConditionProperty();
		PropertyType GetSelectedConditionPropertyType();
		ConditionType GetSelectedConditionType(); 
	};
}

