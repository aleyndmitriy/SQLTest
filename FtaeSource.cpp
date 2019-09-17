#include"FtaeSource.h"
#include"OdsErr.h"
#include"Log.h"

FtaeSource::FtaeSource(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo, const std::shared_ptr<DrvFtaeAlarm::ConditionRecordsDAO>& conditionRecords): configurator(settingsDataSource, [this]()->ODS::UI::IAbstractUIFacrory* {
	if (m_pHost)
		return (ODS::UI::IAbstractUIFacrory*) (m_pHost->GetInterface(ODS::IPluginHost::IID_UI_FACTORY));
	return 	nullptr;
}), browser(settingsDataSource,databaseInfo),server(settingsDataSource,databaseInfo, conditionRecords), m_pRegInfo(nullptr), m_pHost(nullptr)
{

}

FtaeSource::~FtaeSource()
{
	m_pRegInfo = nullptr;
	m_pHost = nullptr;
}


int FtaeSource::Attach(const ODS::IPluginHost* pHost)
{
	if (!pHost)
		return ODS::ERR::BAD_PARAM;

	m_pHost = (ODS::IPluginHost*)pHost;
	ODS::System::ILog* pILog =
		static_cast<ODS::System::ILog*>(const_cast<ODS::IPluginHost*>(pHost)->GetInterface(ODS::IPluginHost::IID_LOG));

	DrvFtaeAlarm::Log::GetInstance()->Init(pILog);
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
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Source Detach"));
	return ODS::ERR::OK;
}

ODS::IPropertySet* FtaeSource::GetPropertySet()
{
	if (m_pRegInfo)
	{
		DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Source Get Property set"));
		ODS::RegisterInfo* pInfo = (ODS::RegisterInfo*)m_pRegInfo;

		return pInfo->m_pPropertySet;
	}

	return NULL;
}

void* FtaeSource::GetInterface(int nIfcId)
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