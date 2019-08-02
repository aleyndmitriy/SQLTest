#include"PropertySet.h"
#include "OdsErr.h"
#include "Property.h"
#include "PropertyInfo.h"

PropertySet::PropertySet():propertyList()
{

}

PropertySet::~PropertySet()
{
	propertyList.clear();
}


void PropertySet::GetPropertyInfoList(ODS::PropertyInfo** ppPropInfoList, ULONG* pulCount) const
{
	*ppPropInfoList = nullptr;
	if (propertyList.empty()) {
		*pulCount = 0;
		return;
	}
	ULONG ulCount = propertyList.size();
	ODS::PropertyInfo* pList = new ODS::PropertyInfo[ulCount];
	if (pList)
	{
		std::map<ULONG, ODS::Property>::const_iterator iter = propertyList.cbegin();
		for(ULONG i = 0; iter != propertyList.cend(); iter++, i++)
		{
			pList[i].SetId(iter->second.GetId());

			TCHAR* szName = NULL;
			iter->second.GetName(&szName);
			if (szName)
			{
				pList[i].SetName(szName);
				iter->second.DestroyString(szName);
			}

			VARTYPE varType;
			ULONG ulValueType = iter->second.GetValueType(&varType);
			pList[i].SetValueType(ulValueType, varType);

			pList[i].SetFlag(ODS::Property::PROP_FLAG_ACCESS_READ_ONLY,
				iter->second.GetFlag(ODS::Property::PROP_FLAG_ACCESS_READ_ONLY));
		}

		*ppPropInfoList = pList;
		*pulCount = ulCount;
	}
}

void PropertySet::DestroyPropertyInfoList(ODS::PropertyInfo* pPropInfoList, ULONG ulCount) const
{
	delete[] pPropInfoList;
}

void PropertySet::GetPropertyList(ODS::Property** ppPropList, ULONG* pulCount) const
{
	*ppPropList = 0;
	if (propertyList.empty()) {
		*pulCount = 0;
		return;
	}
	ULONG ulCount = propertyList.size();

	ODS::Property* pList = new ODS::Property[ulCount];
	if (pList)
	{
		std::map<ULONG, ODS::Property>::const_iterator iter = propertyList.cbegin();
		for (ULONG i = 0; iter != propertyList.cend(); iter++, i++)
		{
			pList[i] = iter->second;
		}
		*ppPropList = pList;
		*pulCount = ulCount;
	}
}

void PropertySet::DestroyPropertyList(ODS::Property* pPropList, ULONG ulCount) const
{
	delete[] pPropList;
}

INT PropertySet::GetProperty(ULONG ulPropId, ODS::Property* pProperty) const
{
	std::map<ULONG, ODS::Property>::const_iterator iter = propertyList.find(ulPropId);
	if (iter != propertyList.cend())
	{
		*pProperty = iter->second;
	}
	return ODS::ERR::OK;
}

INT PropertySet::GetProperty(const TCHAR* pPropName, ODS::Property* pProperty) const
{
	if (!pPropName)
		return ODS::ERR::BAD_PARAM;

	std::map<ULONG, ODS::Property>::const_iterator iter = propertyList.cbegin();

	_tcslwr_s((TCHAR*)pPropName, _tcslen(pPropName));

	while(iter!= propertyList.cend())
	{
		TCHAR* szName = NULL;
		iter->second.GetName(&szName);
		if (szName)
		{
			_tcslwr_s(szName, _tcslen(szName));
			if (strncmp(pPropName, szName, _tcslen(szName)) == 0) {
				*pProperty = iter->second;
			}
			iter->second.DestroyString(szName);
		}
		++iter;
	}
	return ODS::ERR::OK;
}

INT PropertySet::SetProperty(const ODS::Property* pProperty)
{
	ULONG ulId = pProperty->GetId();

	std::map<ULONG, ODS::Property>::const_iterator iter = propertyList.find(ulId);
	if (iter != propertyList.cend())
	{
		if (iter->second.GetFlag(ODS::Property::PROP_FLAG_ACCESS_READ_ONLY))
			return ODS::ERR::ACCESS_DENIED;

		propertyList.erase(iter);
		std::pair<std::map<ULONG, ODS::Property>::const_iterator,bool> res = propertyList.insert(std::make_pair<ULONG, ODS::Property>(ULONG{ ulId }, ODS::Property(*pProperty)));
		if (res.second) {
			return ODS::ERR::OK;
		}
		else {
			return ODS::ERR::CREATE_ERR;
		}
	}
	return ODS::ERR::OK;
}

INT PropertySet::AddProperty(const ULONG ulId, const TCHAR* szName, const TCHAR* szValue)
{
	ODS::Property prop;

	prop.SetId(ulId);
	prop.SetName(szName);
	prop.SetStrValue(szValue);
	std::pair<std::map<ULONG, ODS::Property>::const_iterator, bool> res = propertyList.insert(std::make_pair<ULONG, ODS::Property>(ULONG{ ulId }, ODS::Property(prop)));
	if (res.second) {
		return ODS::ERR::OK;
	}
	else {
		return ODS::ERR::CREATE_ERR;
	}
}
INT PropertySet::AddProperty(const ULONG ulId, const TCHAR* szName, const VARIANT* pValue)
{
	ODS::Property prop;

	prop.SetId(ulId);
	prop.SetName(szName);
	prop.SetVarValue(pValue);
	std::pair<std::map<ULONG, ODS::Property>::const_iterator, bool> res = propertyList.insert(std::make_pair<ULONG, ODS::Property>(ULONG{ ulId }, ODS::Property(prop)));
	if (res.second) {
		return ODS::ERR::OK;
	}
	else {
		return ODS::ERR::CREATE_ERR;
	}
}