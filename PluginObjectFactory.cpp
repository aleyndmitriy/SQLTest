#include"PluginObjectFactory.h"
#include "Property.h"
#include "FtaeSource.h"
#include"DataSQLServerAccessAssembly.h"

PluginObjectFactory::~PluginObjectFactory()
{
	
}

int PluginObjectFactory::CreateObject(const TCHAR* szObjKey, void* pObjCreationParam, ODS::IPluginObj** ppPluginObj)
{
	int iRes = ODS::ERR::OK;
	std::string wObjKey = std::string(szObjKey);
	std::map<std::string, std::unique_ptr<SDsRegInfo> >::const_iterator itr = regInfoDSList.find(wObjKey);
	if (itr == regInfoDSList.cend()) {
		return ODS::ERR::NOT_FOUND;
	}
	if (itr->second) {
		FtaeSource* source = new FtaeSource(DrvFtaeAlarm::DataSQLServerAccessAssembly::instance().GetDataBaseEngine(), DrvFtaeAlarm::DataSQLServerAccessAssembly::instance().GetSettingDataSource(), DrvFtaeAlarm::DataSQLServerAccessAssembly::instance().GetDatabaseInfoSQLServerDao(), DrvFtaeAlarm::DataSQLServerAccessAssembly::instance().GetConditionRecordSQLServerDao());
		if (source) {
			source->SetRegInfo(itr->second.get());
			*ppPluginObj = source;
		}
		else {
			iRes = ODS::ERR::MEMORY_ALLOCATION_ERR;
		}
	}
	else {
		iRes = ODS::ERR::BAD_PARAM;
	}
	return iRes;
}

int PluginObjectFactory::DestroyObject(ODS::IPluginObj* pPluginObj)
{
	int iRes = ODS::ERR::OK;

	if (pPluginObj)
	{
		FtaeSource* pDSource = (FtaeSource*)pPluginObj;

		delete pDSource;
	}
	else
	{
		iRes = ODS::ERR::BAD_PARAM;
	}

	return iRes;
}

void* PluginObjectFactory::GetInterface(int nIfcId)
{
	return this;
}

SDsRegInfo& PluginObjectFactory::at(std::string name)
{
	return *(regInfoDSList.at(name));
}

bool PluginObjectFactory::InsertRegisterInfo(std::string name, std::unique_ptr<SDsRegInfo>&& reg)
{
	std::pair<std::map<std::string, std::unique_ptr<SDsRegInfo> >::const_iterator, bool> res = regInfoDSList.insert(std::make_pair<std::string,std::unique_ptr<SDsRegInfo> >(std::string(name), std::move(reg)));
	return res.second;
}