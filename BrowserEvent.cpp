#include"BrowserEvent.h"
#include"OdsErr.h"
#include"StatementCondition.h"
#include"Property.h"
#include "SQLServerType.h"
#include"Constants.h"
#include"Log.h"

BrowserEvent::BrowserEvent(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo):_settingsDataSource(settingsDataSource),_databaseInfo(databaseInfo),cfgString()
{

}

BrowserEvent::~BrowserEvent()
{
	_databaseInfo.reset();
	_settingsDataSource.reset();
	cfgString.clear();
}

int BrowserEvent::Init(TCHAR* szCfgString)
{
	int iRes = ODS::ERR::OK;
	cfgString = szCfgString;
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Browse Init, %s"), (0 == szCfgString) ? _T("") : szCfgString);
	return iRes;
}

int  BrowserEvent::Shut()
{
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Browse Shut"));
	_databaseInfo.reset();
	_settingsDataSource.reset();
	cfgString.clear();
	return ODS::ERR::OK;
}


void* BrowserEvent::GetInterface(int nIfcId)
{
	return NULL;
}

int BrowserEvent::GetExternalFilterList(TCHAR*** ppszFilterList, ULONG* pulCount)
{

	return ODS::ERR::OK;
}


int BrowserEvent::GetFilterList(TCHAR*** ppszFilterList, ULONG* pulCount)
{
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetFilterList"));
	std::vector<std::string> filterNameList;
	if (!cfgString.empty())
	{
		size_t len = cfgString.size();
		if (_settingsDataSource) {
			filterNameList = _settingsDataSource->GetFiltersName(cfgString.c_str(), len);
		}
		else {
			return ODS::ERR::OK;
		}
	}
	else {
		return ODS::ERR::OK;
	}
	if (filterNameList.empty()) {
		filterNameList.push_back(std::string("Default"));
	}
	ULONG count = filterNameList.size();
	*pulCount = count;
	*ppszFilterList = new TCHAR * [count];
	for (UINT ind = 0; ind < count; ind++)
	{
		UINT uiSize = filterNameList[ind].size() + 1;
		(*ppszFilterList)[ind] = new TCHAR[uiSize];
		if ((*ppszFilterList)[ind])
		{
			_tcscpy_s((*ppszFilterList)[ind], uiSize, (LPCSTR)filterNameList[ind].c_str());
		}
	}

	return ODS::ERR::OK;
}

int BrowserEvent::GetAlarmPropertyInfoList(ODS::PropertyInfo** ppPropertyInfoList, ULONG* pulCount)
{
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetAlarmPropertyInfoList"));
	DrvFtaeAlarm::ConnectionAttributes attributes;
	if (!cfgString.empty())
	{
		size_t len = cfgString.size();
		if (_settingsDataSource) {
			_settingsDataSource->LoadAttributesSettingsString(cfgString.c_str(), len, attributes);
		}
		else {
			return ODS::ERR::DB_NO_DATA;
		}
	}
	else {
		return ODS::ERR::DB_NO_DATA;
	}
	
	std::unique_ptr<DrvFtaeAlarm::SQLTable> table = _databaseInfo->GetTableInfo(true, attributes, std::string(), std::string("ConditionEvent"));

	std::map<std::string, DrvFtaeAlarm::PropertyType> properties;
	for (DrvFtaeAlarm::SQLTable::const_iterator itr = table->cbegin(); itr != table->cend(); ++itr) {
		if (itr->first == std::string("EventTimeStamp") || itr->first == std::string("Priority") || itr->first == std::string("Message")) {
			continue;
		}
		std::pair<std::string, DrvFtaeAlarm::PropertyType> pair = std::make_pair<std::string, DrvFtaeAlarm::PropertyType>(std::string(itr->first), DrvFtaeAlarm::PropertyTypeFromString(itr->second));
		properties.insert(pair);
	}
	*ppPropertyInfoList = new ODS::PropertyInfo[properties.size()];
	if (*ppPropertyInfoList)
	{
		ULONG i = 0;
		for (std::map< std::string, DrvFtaeAlarm::PropertyType>::const_iterator itr = properties.cbegin(); itr != properties.cend(); ++itr) {
			(*ppPropertyInfoList + i)->SetId(PROP_START_ID + i);
			(*ppPropertyInfoList + i)->SetName(itr->first.c_str());
			switch (itr->second)
			{
			case DrvFtaeAlarm::PropertyType::PROPTYPE_NUMERIC:
				(*ppPropertyInfoList + i)->SetValueType(ODS::Property::PROP_VALUE_TYPE_VAR, VT_R8);
				break;
			case DrvFtaeAlarm::PropertyType::PROPTYPE_TEXT:
				(*ppPropertyInfoList + i)->SetValueType(ODS::Property::PROP_VALUE_TYPE_STR, VT_BSTR);
				break;
			case DrvFtaeAlarm::PropertyType::PROPTYPE_BOOLEAN:
				(*ppPropertyInfoList + i)->SetValueType(ODS::Property::PROP_VALUE_TYPE_VAR, VT_BOOL);
				break;
			case DrvFtaeAlarm::PropertyType::PROPTYPE_DATE:
				(*ppPropertyInfoList + i)->SetValueType(ODS::Property::PROP_VALUE_TYPE_VAR, VT_DATE);
				break;
			default:
				(*ppPropertyInfoList + i)->SetValueType(ODS::Property::PROP_VALUE_TYPE_STR, VT_BSTR);
				break;
			}
			++i;
		}
	}
	*pulCount = properties.size();
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetAlarmPropertyInfoList: property list size: %d"),properties.size());
	return  ODS::ERR::OK;
}

int BrowserEvent::DestroyFilterList(TCHAR** ppFilterList, ULONG ulCount)
{
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetAlarmPropertyInfoList"));
	if (ppFilterList)
	{
		for (ULONG ind = 0; ind < ulCount; ind++)
		{
			if (ppFilterList[ind])
				delete[] ppFilterList[ind];
		}

		delete[] ppFilterList;
	}
	else
	{
		return ODS::ERR::BAD_PARAM;
	}

	return ODS::ERR::OK;
}

int BrowserEvent::DestroyAlarmPropertyInfoList(ODS::PropertyInfo* pPropertyInfoList, ULONG ulCount)
{
	delete[] pPropertyInfoList;

	return ODS::ERR::OK;
}