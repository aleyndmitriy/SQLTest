#pragma once
#include"IPluginObjectFactory.h"
#include"IPluginHost.h"
#include"OdsErr.h"
#include"Utils.h"
#include<string>
#include<map>
#include<memory>
struct SDsRegInfo
{
	std::string m_szVendorName;
	std::string m_szDataSourceName;
	std::string m_szDescription;

	std::string m_szClientName;
	std::string m_szBrowserDLL;
	std::string m_szClientDLL;

	std::string m_szDrvName;
	std::string m_szParam;

	ODS::RegisterInfo m_InfoDS;

	//CPropSet m_PropSet;
};

class PluginObjectFactory final: public ODS::IPluginObjectFactory
{
public:
	int CreateObject(const TCHAR* szObjKey, void* pObjCreationParam, ODS::IPluginObj** ppPluginObj) override;
	int DestroyObject(ODS::IPluginObj* pPluginObj) override;
	void* GetInterface() override;
	SDsRegInfo& at(std::string name);
	bool InsertRegisterInfo(std::string name, std::unique_ptr<SDsRegInfo>&& reg);
private:
	std::map<std::string, std::unique_ptr<SDsRegInfo> > regInfoDSList;
};