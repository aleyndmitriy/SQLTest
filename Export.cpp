#include"Export.h"
#include"PluginObjectFactory.h"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

PluginObjectFactory gObjectFactory;

int InitPlugin(void* pParam, ODS::IPluginHost* pPluginHost)
{
	if (pParam == nullptr ||  pPluginHost == nullptr) {
		return ODS::ERR::INIT_ERR;
	}
	std::unique_ptr<SDsRegInfo> pRegInfo = std::make_unique<SDsRegInfo>();
	pRegInfo->m_szDataSourceName = std::string("Ftae Event Driver");
	pRegInfo->m_szVendorName = std::string("FTAE");
	pRegInfo->m_szDescription = std::string("Ftae Event Driver Description");
	pRegInfo->m_szClientName = pRegInfo->m_szDataSourceName + std::string(" Conditional Events");
	std::string objKey = pRegInfo->m_szClientName + std::string("Drv_Ftae_Alarm");
	_tcscpy_s(pRegInfo->m_InfoDS.m_szObjKey, MAX_PATH, objKey.c_str());
	pRegInfo->m_InfoDS.m_pFactory = &gObjectFactory;
	pRegInfo->m_InfoDS.m_pObjCreationParam = &(pRegInfo->m_InfoDS);
	pRegInfo->m_InfoDS.m_Version.m_nMajor = VERSION_MAJOR;
	pRegInfo->m_InfoDS.m_Version.m_nMinor = VERSION_MINOR;
	gObjectFactory.InsertRegisterInfo(objKey, std::move(pRegInfo));
	pPluginHost->RegisterObject(pParam, &(gObjectFactory.at(objKey).m_InfoDS));
	return ODS::ERR::OK;
}