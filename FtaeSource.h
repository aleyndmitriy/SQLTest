#pragma once
#include"ISourceFtaeEvent.h"
#include"UISettingsConfigurator.h"
#include"BrowserEvent.h"
#include"IPluginHost.h"
#include"FtaeServer.h"

class FtaeSource final : public ODS::ISourceFtaeEvent
{
public:
	FtaeSource() = delete;
	FtaeSource(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo);
	FtaeSource(const FtaeSource& src) = delete;
	FtaeSource& operator=(const FtaeSource& src) = delete;
	FtaeSource(FtaeSource&& rhs) = delete;
	FtaeSource& operator=(FtaeSource&& src) = delete;
	~FtaeSource();
	void* GetInterface() override;
	int Attach(const ODS::IPluginHost* pHost) override;
	int Detach() override;
	void SetRegInfo(LPVOID pRegInfo);
	ODS::IPropertySet* GetPropertySet() override;
	ODS::IDsConfiguratorEvent* GetConfigurator() override;
	ODS::IServerFtae* GetServer() override;
	ODS::IBrowserEvent* GetBrowser() override;
private:
	std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> _settingsDataSource;
	std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO> _databaseInfo;
	UISettingsConfigurator configurator;
	BrowserEvent browser;
	FtaeServer server;
	LPVOID m_pRegInfo;
	ODS::IPluginHost* m_pHost;
};