#pragma once
#include "IDsConfiguratorEvent.h"
#include "ISettingsDataSource.h"
#include<memory>

class UISettingsConfigurator final : public ODS::IDsConfiguratorEvent {
public:
	UISettingsConfigurator() = delete;
	UISettingsConfigurator(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> &settingsDataSource);
	~UISettingsConfigurator();
	UISettingsConfigurator(const UISettingsConfigurator& src) = delete;
	UISettingsConfigurator(UISettingsConfigurator&& src) = delete;
	UISettingsConfigurator& operator=(const UISettingsConfigurator& rhs) = delete;
	UISettingsConfigurator& operator=(UISettingsConfigurator&& rhs) = delete;
	int ConfigureFilters(TCHAR* szCfgInString, TCHAR** pszCfgOutString) override;
	int Configure(const TCHAR* szCfgInString, TCHAR** pszCfgOutString) override;
	int DestroyString(TCHAR* szCfgString) override;
	void* GetInterface() override;
private:
	std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> _settingsDataSource;
};