#include"PluginObjectFactory.h"
#include "Property.h"

int PluginObjectFactory::CreateObject(const TCHAR* szObjKey, void* pObjCreationParam, ODS::IPluginObj** ppPluginObj)
{
	std::string wObjKey = std::string(szObjKey);
	std::map<std::string, std::unique_ptr<SDsRegInfo> >::const_iterator itr = regInfoDSList.find(wObjKey);
	if (itr == regInfoDSList.cend()) {
		return ODS::ERR::NOT_FOUND;
	}
	
	return 0;
}

int PluginObjectFactory::DestroyObject(ODS::IPluginObj* pPluginObj)
{
	return 0;
}

void* PluginObjectFactory::GetInterface()
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