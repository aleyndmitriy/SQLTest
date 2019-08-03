#pragma once
#include "IDsConfiguratorEvent.h"
#include "XMLSettingsDataSource.h"

class UISettingsConfigurator final : public ODS::IDsConfiguratorEvent, public DrvFtaeAlarm::XMLSettingsDataSource {
public:
	UISettingsConfigurator() = default;
	~UISettingsConfigurator() {};
	UISettingsConfigurator(const UISettingsConfigurator& src) = delete;
	UISettingsConfigurator(UISettingsConfigurator&& src) = delete;
	UISettingsConfigurator& operator=(const UISettingsConfigurator& rhs) = delete;
	UISettingsConfigurator& operator=(UISettingsConfigurator&& rhs) = delete;
	int ConfigureFilters(TCHAR* szCfgInString, TCHAR** pszCfgOutString) override;
	int Configure(const TCHAR* szCfgInString, TCHAR** pszCfgOutString) override;
	int DestroyString(TCHAR* szCfgString) override;
	void* GetInterface() override;
};