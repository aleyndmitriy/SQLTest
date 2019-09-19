#include"UISettingsConfigurator.h"
#include "OdsErr.h"
#include"SettingsInitializer.h"
#include "Constants.h"
#include <sstream>

UISettingsConfigurator::UISettingsConfigurator(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, std::function<ODS::UI::IAbstractUIFacrory* (void)> uiFactoryGetter):_settingsDataSource(settingsDataSource),_uiFactoryGetter(uiFactoryGetter), _hParentWindow(nullptr)
{

}

UISettingsConfigurator::~UISettingsConfigurator() 
{
	_settingsDataSource.reset();
}

int UISettingsConfigurator::Configure(const TCHAR* szCfgInString, TCHAR** pszCfgOutString)
{
	int iRes = ODS::ERR::FILE;
	std::shared_ptr<DrvFtaeAlarm::ConnectionAttributes> attributes = std::make_shared<DrvFtaeAlarm::ConnectionAttributes>();
	std::shared_ptr<std::map<std::string, std::vector<DrvFtaeAlarm::StatementCondition> > > filters = std::make_shared<std::map<std::string, std::vector<DrvFtaeAlarm::StatementCondition> > >();
	if (szCfgInString != NULL)
	{
		size_t len = _tcslen(szCfgInString);
		if (_settingsDataSource) {
			_settingsDataSource->LoadSettingsString(szCfgInString, len,*attributes,*filters);
		}
	}
	int isOk = DrvFtaeAlarm::SettingsInitializer::CreateModule(GetModuleHandle("Drv_Ftae_HdaAlarm.dll"), _uiFactoryGetter,&_hParentWindow,attributes,filters);
	if (isOk) {
		std::ostringstream outStream;
		if (_settingsDataSource) {
			_settingsDataSource->SaveSettingsString(*attributes,*filters,outStream);
			std::string outString = outStream.str();
			char* outStr = new char[outString.length() + 1];
			_tcscpy_s(outStr, outString.length() + 1, outString.c_str());
			*pszCfgOutString = outStr;
			iRes = ODS::ERR::OK;
		}
	}
	else {
		if (szCfgInString != NULL)
		{
			size_t len = _tcslen(szCfgInString);
			char* outStr = new char[len + 1];
			_tcscpy_s(outStr, len + 1, szCfgInString);
			*pszCfgOutString = outStr;
			iRes = ODS::ERR::OK;
		}
	}
	return iRes;
}

int UISettingsConfigurator::DestroyString(TCHAR* szCfgString)
{
	if (szCfgString)
		delete[] szCfgString;

	return ODS::ERR::OK;
}


int UISettingsConfigurator::ConfigureFilters(TCHAR* szCfgInString, TCHAR** pszCfgOutString)
{
	int iRes = ODS::ERR::OK;


	return iRes;
}

void* UISettingsConfigurator::GetInterface(int nIfcId)
{
	return NULL;
}

void UISettingsConfigurator::SetOwnerWnd(void* pOwnerWnd) {
	_hParentWindow = (HWND)(pOwnerWnd);
}
