#pragma once
#include"ISourceFtaeEvent.h"
#include"UISettingsConfigurator.h"
#include"BrowserEvent.h"
#include"IPluginHost.h"

class FtaeSource final : public ODS::ISourceFtaeEvent
{
public:
	void* GetInterface() override;
	int Attach(const ODS::IPluginHost* pHost) override;
	int Detach() override;
	void SetRegInfo(LPVOID pRegInfo);
	ODS::IPropertySet* GetPropertySet() override;
	ODS::IDsConfiguratorEvent* GetConfigurator() override;
	ODS::IServerFtae* GetServer() override;
	ODS::IBrowserEvent* GetBrowser() override;
private:
	UISettingsConfigurator configurator;
	BrowserEvent browser;
	LPVOID m_pRegInfo;
	ODS::IPluginHost* m_pHost;
};