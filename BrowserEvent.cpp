#include"BrowserEvent.h"
#include"OdsErr.h"
#include"StatementCondition.h"
#include"Property.h"
BrowserEvent::BrowserEvent():cfgString()
{

}

BrowserEvent::~BrowserEvent()
{
	cfgString.clear();
}

int BrowserEvent::Init(TCHAR* szCfgString)
{
	int iRes = ODS::ERR::OK;

	cfgString = szCfgString;

	return iRes;
}

int  BrowserEvent::Shut()
{
	return ODS::ERR::OK;
}


void* BrowserEvent::GetInterface()
{
	return NULL;
}

int BrowserEvent::GetExternalFilterList(TCHAR*** ppszFilterList, ULONG* pulCount)
{

	return ODS::ERR::OK;
}


int BrowserEvent::GetFilterList(TCHAR*** ppszFilterList, ULONG* pulCount)
{
	if (!cfgString.empty())
	{
		size_t len = cfgString.size();
		LoadXMLString(cfgString.c_str(), len);
	}
	std::vector<std::string> filterNameList = GetFiltersName();
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
	std::map< std::string, DrvFtaeAlarm::PropertyType> properties;
	std::pair<std::string, DrvFtaeAlarm::PropertyType> pair1 = std::make_pair<std::string, DrvFtaeAlarm::PropertyType>(std::string("columnName1"), DrvFtaeAlarm::PropertyType::PROPTYPE_TEXT);
	properties.insert(pair1);
	std::pair<std::string, DrvFtaeAlarm::PropertyType> pair2 = std::make_pair<std::string, DrvFtaeAlarm::PropertyType>(std::string("columnName2"), DrvFtaeAlarm::PropertyType::PROPTYPE_NUMERIC);
	properties.insert(pair2);
	std::pair<std::string, DrvFtaeAlarm::PropertyType> pair3 = std::make_pair<std::string, DrvFtaeAlarm::PropertyType>(std::string("columnName3"), DrvFtaeAlarm::PropertyType::PROPTYPE_BOOLEAN);
	properties.insert(pair3);
	std::pair<std::string, DrvFtaeAlarm::PropertyType> pair4 = std::make_pair<std::string, DrvFtaeAlarm::PropertyType>(std::string("columnName4"), DrvFtaeAlarm::PropertyType::PROPTYPE_DATE);
	properties.insert(pair4);
	std::pair<std::string, DrvFtaeAlarm::PropertyType> pair5 = std::make_pair<std::string, DrvFtaeAlarm::PropertyType>(std::string("columnName4"), DrvFtaeAlarm::PropertyType::PROPTYPE_TEXT);
	properties.insert(pair5);
	*ppPropertyInfoList = new ODS::PropertyInfo[properties.size()];
	if (*ppPropertyInfoList)
	{
		ULONG i = 0;
		for (std::map< std::string, DrvFtaeAlarm::PropertyType>::const_iterator itr = properties.cbegin(); itr != properties.cend(); ++itr) {
			(*ppPropertyInfoList)->SetId(i);
			(*ppPropertyInfoList)->SetName(itr->first.c_str());
			switch (itr->second)
			{
			case DrvFtaeAlarm::PropertyType::PROPTYPE_NUMERIC:
				(*ppPropertyInfoList)->SetValueType(ODS::Property::PROP_VALUE_TYPE_VAR, VT_R8);
				break;
			case DrvFtaeAlarm::PropertyType::PROPTYPE_TEXT:
				(*ppPropertyInfoList)->SetValueType(ODS::Property::PROP_VALUE_TYPE_STR, VT_BSTR);
				break;
			case DrvFtaeAlarm::PropertyType::PROPTYPE_BOOLEAN:
				(*ppPropertyInfoList)->SetValueType(ODS::Property::PROP_VALUE_TYPE_VAR, VT_BOOL);
				break;
			case DrvFtaeAlarm::PropertyType::PROPTYPE_DATE:
				(*ppPropertyInfoList)->SetValueType(ODS::Property::PROP_VALUE_TYPE_VAR, VT_DATE);
				break;
			default:
				(*ppPropertyInfoList)->SetValueType(ODS::Property::PROP_VALUE_TYPE_STR, VT_BSTR);
				break;
			}
		}
		++i;
	}
	*pulCount = properties.size();
	return  ODS::ERR::OK;
}

int BrowserEvent::DestroyFilterList(TCHAR** ppFilterList, ULONG ulCount)
{
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