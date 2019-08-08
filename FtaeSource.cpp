#include"FtaeSource.h"
#include"OdsErr.h"


FtaeSource::FtaeSource(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo):
	_settingsDataSource(settingsDataSource), _databaseInfo(databaseInfo),configurator(settingsDataSource), browser(settingsDataSource,databaseInfo),server(settingsDataSource,databaseInfo), m_pRegInfo(nullptr), m_pHost(nullptr)
{

}

FtaeSource::~FtaeSource()
{
	_settingsDataSource.reset();
	_databaseInfo.reset();
	m_pRegInfo = nullptr;
	m_pHost = nullptr;
}


int FtaeSource::Attach(const ODS::IPluginHost* pHost)
{
	if (!pHost)
		return ODS::ERR::BAD_PARAM;

	m_pHost = (ODS::IPluginHost*)pHost;

	return ODS::ERR::OK;
}

/**
@brief Detach plugin from framework. This is the last operation
on the plugin. It's called before destroying

@return Error code
*/
int FtaeSource::Detach()
{
	m_pHost = NULL;

	return ODS::ERR::OK;
}

ODS::IPropertySet* FtaeSource::GetPropertySet()
{
	if (m_pRegInfo)
	{
		ODS::RegisterInfo* pInfo = (ODS::RegisterInfo*)m_pRegInfo;

		return pInfo->m_pPropertySet;
	}

	return NULL;
}

void* FtaeSource::GetInterface()
{
	return NULL;
}

void FtaeSource::SetRegInfo(LPVOID pRegInfo)
{
	m_pRegInfo = pRegInfo;
}

ODS::IDsConfiguratorEvent* FtaeSource::GetConfigurator()
{
	return &configurator;
}

ODS::IServerFtae* FtaeSource::GetServer()
{
	return &server;
}



ODS::IBrowserEvent* FtaeSource::GetBrowser()
{
	return &browser;
}