#pragma once
#include "IDsConfiguratorEvent.h"
#include "ISettingsDataSource.h"
#include "IAbstractUIFacrory.h"
#include<memory>
#include <functional>
class UISettingsConfigurator final : public ODS::IDsConfiguratorEvent {
public:
	
	UISettingsConfigurator() = delete;
	UISettingsConfigurator(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> &settingsDataSource, std::function<ODS::UI::IAbstractUIFacrory* (void)> uiFactoryGetter);
	~UISettingsConfigurator();
	UISettingsConfigurator(const UISettingsConfigurator& src) = delete;
	UISettingsConfigurator(UISettingsConfigurator&& src) = delete;
	UISettingsConfigurator& operator=(const UISettingsConfigurator& rhs) = delete;
	UISettingsConfigurator& operator=(UISettingsConfigurator&& rhs) = delete;
	int ConfigureFilters(TCHAR* szCfgInString, TCHAR** pszCfgOutString) override;
	int Configure(const TCHAR* szCfgInString, TCHAR** pszCfgOutString) override;
	int DestroyString(TCHAR* szCfgString) override;
	void* GetInterface(int nIfcId) override;
	void SetOwnerWnd(void* pOwnerWnd) override;
private:
	std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> _settingsDataSource;
	std::function<ODS::UI::IAbstractUIFacrory* (void)> _uiFactoryGetter;
	HWND _hParentWindow;
};