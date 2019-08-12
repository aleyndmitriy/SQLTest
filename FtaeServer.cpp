#include"FtaeServer.h"
#include"OdsErr.h"
#include "Alarm.h"
#include"AlarmFilter.h"
#include "AlarmPropertyId.h"
#include "HdaFunctionTypes.h"
#include "HdaFunctionParam.h"
#include "HdaCommand.h"
#include"HdaCommandParam.h"
#include "SQLServerType.h"
#include"Constants.h"
#include"StatementCondition.h"

FtaeServer::FtaeServer(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo, const std::shared_ptr<DrvFtaeAlarm::ConditionRecordsDAO>& recordsInfo):_settingsDataSource(settingsDataSource), _databaseInfo(databaseInfo), _recordsInfo(recordsInfo), cfgString()
{

}

FtaeServer::~FtaeServer()
{
	_settingsDataSource.reset();
	_databaseInfo.reset();
	cfgString.clear();
}

int FtaeServer::Init(TCHAR* szCfgString)
{
	int iRes = ODS::ERR::OK;

	cfgString = szCfgString;

	return iRes;
}

int  FtaeServer::Shut()
{
	return ODS::ERR::OK;
}

void* FtaeServer::GetInterface()
{
	return NULL;
}

int FtaeServer::IsHdaFunctionSupported(int nFuncType)
{
	if (nFuncType == ODS::HdaFunctionType::ALARM_LIST)
		return 1;

	return 0;
}

int FtaeServer::Execute(ODS::HdaCommand* pCommand, ODS::HdaCommandResult** ppResult)
{
	int iRes = ODS::ERR::OK;

	if (!pCommand || !ppResult)
		return ODS::ERR::BAD_PARAM;

	ODS::HdaFunction** pList = nullptr;
	int nCount = 0;

	SYSTEMTIME stTimeFrom = { 0 };
	SYSTEMTIME stTimeTo = { 0 };
	iRes = GetCmdParameterList(pCommand, stTimeFrom, stTimeTo);
	if (iRes == ODS::ERR::OK && pCommand)
	{
		iRes = pCommand->GetFunctionList(&pList, &nCount);
	}
	if (iRes == ODS::ERR::OK)
	{
		ODS::HdaCommandResult* pResult = new ODS::HdaCommandResult;
		if (pResult) {
			for (int ind = 0; ind < nCount; ind++) {
				ODS::HdaFunctionResult* pFuncResult = NULL;
				int nFnType = pList[ind]->GetType();
				if (nFnType == ODS::HdaFunctionType::ALARM_LIST) {
					std::string sqc;
					std::vector<DrvFtaeAlarm::PRIORITY_FILTER> filterList;
					std::vector<std::string> staticFilterList;
					if (ODS::ERR::OK == GetFuncParameterList(pList[ind], sqc, filterList, staticFilterList)) {
						std::vector<DrvFtaeAlarm::Record> records = LoadEvents(staticFilterList);
						if (!records.empty()) {
							pFuncResult = new ODS::HdaFunctionResultAlarmList;
							if (pFuncResult) {
								iRes = BuildFuncResult(pFuncResult, records);
							}
							else {
								iRes = ODS::ERR::MEMORY_ALLOCATION_ERR;
								break;
							}
						}
						
					}
				}
				else {
					pFuncResult = new ODS::HdaFunctionResultObject;
				}
				if (pFuncResult) {
					pFuncResult->SetContext(pList[ind]->GetContext());
					pResult->AddFunctionResult(pFuncResult);
				}
				else {
					iRes = ODS::ERR::MEMORY_ALLOCATION_ERR;
					break;
				}
			}
			pCommand->DestroyFunctionList(pList, nCount);
			*ppResult = pResult;
		}
		else {
			iRes = ODS::ERR::MEMORY_ALLOCATION_ERR;
		}
	}
	return iRes;
}

int FtaeServer::DestroyResult(ODS::HdaCommandResult* pResult)
{
	delete pResult;
	return 0;
}

int FtaeServer::GetCmdParameterList(ODS::HdaCommand* pCommand, SYSTEMTIME& rStartTime, SYSTEMTIME& rEndTime)
{
	int iRes = ODS::ERR::OK;

	ODS::HdaCommandParam** ppCmdParamList = NULL;
	int nCount = 0;

	if (!pCommand)
		return ODS::ERR::BAD_PARAM;

	iRes = pCommand->GetParamList(&ppCmdParamList, &nCount);
	if (iRes == ODS::ERR::OK)
	{
		for (int i = 0; i < nCount; i++)
		{
			if (ppCmdParamList[i]->GetType() == ODS::HdaCommandParam::TYPE_TIME_PERIOD)
			{
				ODS::HdaCommandParamTimePeriod* pTimePeriod = (ODS::HdaCommandParamTimePeriod*)ppCmdParamList[i];

				SYSTEMTIME stStart;
				SYSTEMTIME stEnd;
				SYSTEMTIME currTime;

				if (ODS::ERR::OK == pTimePeriod->GetTimePeriod(&stStart, &stEnd, &currTime))
				{
					rStartTime = stStart;
					rEndTime = stEnd;

					break;
				}
			}
		}
		pCommand->DestroyParamList(ppCmdParamList, nCount);
	}
	return iRes;
}

int FtaeServer::GetFuncParameterList(ODS::HdaFunction* pFunc, std::string& szSqc, std::vector<DrvFtaeAlarm::PRIORITY_FILTER>& filterList, std::vector<std::string>& staticFilterList)
{
	int iRes = ODS::ERR::OK;

	ODS::HdaFunctionParam** ppFPList = NULL;
	int nCountParam = 0;

	iRes = pFunc->GetParameterList(&ppFPList, &nCountParam);
	if (ODS::ERR::OK == iRes)
	{
		PVOID handle = NULL;

		for (int i = 0; i < nCountParam; i++)
		{
			switch (ppFPList[i]->GetType())
			{
			case ODS::HdaFunctionParam::TYPE_SPEC_POINT:
			{
				ODS::HdaFunctionParamSpecPoint* pSPParam = (ODS::HdaFunctionParamSpecPoint*)ppFPList[i];
			}
			break;

			case ODS::HdaFunctionParam::TYPE_SQL:
			{
				ODS::HdaFunctionParamSql* pParam = (ODS::HdaFunctionParamSql*)ppFPList[i];

				TCHAR* pText = 0;
				pParam->GetText(&pText);
				if (pText)
					szSqc = pText;

				pParam->DestroyString(pText);
			}
			break;

			case ODS::HdaFunctionParam::TYPE_ALARM_FILTER_LIST:
			{
				ODS::AlarmFilter** ppFilterList = NULL;
				int nAFCount = 0;

				ODS::HdaFunctionParamAlarmFilterList* pSPParam = (ODS::HdaFunctionParamAlarmFilterList*)ppFPList[i];

				pSPParam->GetFilterList(&ppFilterList, &nAFCount);

				for (int i_AF = 0; i_AF < nAFCount; i_AF++)
				{
					if (ppFilterList[i_AF])
					{
						if (ODS::AlarmFilter::FT_DYNAMIC == ppFilterList[i_AF]->GetType())
						{
							ODS::AlarmFilterDynamic* pFilter = (ODS::AlarmFilterDynamic*)ppFilterList[i_AF];

							ODS::AlarmFilterComponent* pAFComponentList = NULL;
							int nAFComponentCount = 0;

							if (ODS::ERR::OK == pFilter->GetFilterComponentList(&pAFComponentList, &nAFComponentCount))
							{
								DrvFtaeAlarm::PRIORITY_FILTER filter;

								for (int i_AFC = 0; i_AFC < nAFComponentCount; i_AFC++)
								{
									VARTYPE vtVarType = VT_NULL;
									ODS::Property propFrom;
									ODS::Property propTo;

									pAFComponentList[i_AFC].GetRange(&propFrom, &propTo);

									if (ODS::AlarmProperty::ID_PRIORITY == propFrom.GetId() &&
										ODS::AlarmProperty::ID_PRIORITY == propTo.GetId() &&
										ODS::Property::PROP_VALUE_TYPE_VAR == propFrom.GetValueType(&vtVarType) &&
										ODS::Property::PROP_VALUE_TYPE_VAR == propTo.GetValueType(&vtVarType))
									{
										VARIANT vValFrom;
										VARIANT vValTo;

										::VariantInit(&vValFrom);
										::VariantInit(&vValTo);

										propFrom.GetVarValue(&vValFrom);
										propTo.GetVarValue(&vValTo);

										filter.m_PriorityFrom = VariantToUSHORT(&vValFrom);
										filter.m_PriorityTo = VariantToUSHORT(&vValTo);

										::VariantClear(&vValFrom);
										::VariantClear(&vValTo);

										filterList.push_back(filter);
									}
								}
							}

							if (pFilter && pAFComponentList)
								pFilter->DestroyFilterComponentList(pAFComponentList, nAFComponentCount);
						}
						if (ODS::AlarmFilter::FT_EXTERNAL == ppFilterList[i_AF]->GetType())
						{
							ODS::AlarmFilterStatic* pFilter = (ODS::AlarmFilterStatic*)ppFilterList[i_AF];
							ODS::AlarmFilterComponent* pAFComponentList = NULL;
							int nAFComponentCount = 0;
							TCHAR* name = NULL;
							pFilter->GetName(&name);

							staticFilterList.push_back(name);
						}
					}
				}
				pSPParam->DeleteFilterList(ppFilterList, nAFCount);
			}
			break;
			} // end of switch
		}

		// clear allocated memory
		pFunc->DestroyParameterList(ppFPList, nCountParam);
	}

	return iRes;
}

int FtaeServer::BuildFuncResult(ODS::HdaFunctionResult* pFuncResult, const std::vector<DrvFtaeAlarm::Record>& rRecordList)
{
	if (rRecordList.empty()) {
		return ODS::ERR::DB_NO_DATA;
	}
	int iAddRecCount = 0;
	ODS::Alarm* pAlarmList = new ODS::Alarm[rRecordList.size()];
	if (!pAlarmList) {
		return ODS::ERR::MEMORY_ALLOCATION_ERR;
	}
	ODS::HdaFunctionResultAlarmList* pFR = (ODS::HdaFunctionResultAlarmList*)pFuncResult;
	for (std::vector<DrvFtaeAlarm::Record>::const_iterator recordsIterator = rRecordList.cbegin(); recordsIterator != rRecordList.cend(); ++recordsIterator) {
		size_t nAlarmPropCount = recordsIterator->GetColumnNumber();
		std::vector<ODS::Property> listProp;
		unsigned int index = 0;
		for (DrvFtaeAlarm::Record::const_iterator columnIter = recordsIterator->cbegin(); columnIter != recordsIterator->cend(); ++columnIter) {
			ODS::Property prop;
			SetODSProperty(prop, PROP_START_ID + index,columnIter->first.c_str(),columnIter->second.second,columnIter->second.first);
			++index;
			listProp.push_back(prop);
		}
		size_t count = listProp.size();
		ODS::Property* pPropList = new ODS::Property[count];
		if (pPropList)
		{
			for (size_t idx = 0; idx < count; idx++)
				pPropList[idx] = listProp[idx];

			pAlarmList[iAddRecCount].SetPropertyList(pPropList, count);

			iAddRecCount++;

			delete[] pPropList;
		}
	}

	pFR->SetAlarmList(pAlarmList, iAddRecCount);

	// clear memory
	delete[] pAlarmList;
	return ODS::ERR::OK;
}

std::vector<DrvFtaeAlarm::Record> FtaeServer::LoadEvents(std::vector<std::string> filters)
{
	std::vector<DrvFtaeAlarm::Record> records;
	if (!_settingsDataSource) {
		return records;
	}
	DrvFtaeAlarm::ConnectionAttributes attributes;
	if (!_settingsDataSource->Load(attributes)) {
		return records;
	}
	std::map<std::pair<std::string, bool>, std::vector<DrvFtaeAlarm::StatementCondition> > loadedFilters;
	if (!_settingsDataSource->Load(loadedFilters)) {
		return records;
	}
	std::vector<DrvFtaeAlarm::StatementCondition> conditions;

	for (std::vector<std::string>::const_iterator itr = filters.cbegin(); itr != filters.cend(); ++itr) {
		for (std::map<std::pair<std::string, bool>, std::vector<DrvFtaeAlarm::StatementCondition> >::const_iterator mapItr = loadedFilters.cbegin();
			mapItr != loadedFilters.cend(); ++mapItr) {
			if (mapItr->first.first == *itr) {
				conditions.insert(conditions.cend(), mapItr->second.cbegin(), mapItr->second.cend());
			}
		}
	}
	std::unique_ptr<DrvFtaeAlarm::SQLTable> table = _databaseInfo->GetTableInfo(attributes, std::string("FTAE"), std::string("ConditionEvent"));
	records = _recordsInfo->GetRecords(*table, attributes, conditions);
	
	return records;
}

USHORT VariantToUSHORT(VARIANT* pvValue)
{
	// convert data to string
	switch (V_VT(pvValue))
	{
	case VT_I2:
		return (USHORT)V_I2(pvValue);

	case VT_I4:
		return (USHORT)V_I4(pvValue);

	case VT_R4:
		return (USHORT)V_R4(pvValue);

	case VT_R8:
		return (USHORT)V_R8(pvValue);

	case VT_DATE:
		return (USHORT)V_DATE(pvValue);

	case VT_ERROR:
		return (USHORT)V_ERROR(pvValue);

	case VT_BOOL:
		return (USHORT)V_BOOL(pvValue);

	case VT_I1:
		return (USHORT)V_I1(pvValue);

	case VT_UI1:
		return (USHORT)V_UI1(pvValue);

	case VT_UI2:
		return (USHORT)V_UI2(pvValue);

	case VT_UI4:
		return (USHORT)V_UI4(pvValue);

	case VT_INT:
		return (USHORT)V_INT(pvValue);

	case VT_UINT:
		return (USHORT)V_UINT(pvValue);

	default:
		return (USHORT)V_NONE(pvValue);

	}
}

void SetODSProperty(ODS::Property& prop, ULONG ulId, const TCHAR* szName, const std::string& szValue, short type)
{
	VARIANT vValue;
	float val = 0.0;
	prop.SetFlag(ODS::Property::PROP_FLAG_ACCESS_READ_ONLY, true);
	prop.SetId(ulId);
	prop.SetName(szName);
	switch (type)
	{
	case SQL_C_CHAR:
	case SQL_C_GUID:
	case SQL_C_BINARY:
		prop.SetStrValue(szValue.c_str());
		break;
	case SQL_FLOAT:
		::VariantInit(&vValue);
		vValue.vt = VT_R8;
		vValue.dblVal = std::stod(szValue);
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_FLOAT:
		::VariantInit(&vValue);
		vValue.vt = VT_R8;
		val = std::stof(szValue);
		vValue.fltVal = val;
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_BIT:
		::VariantInit(&vValue);
		vValue.vt = VT_BOOL;
		vValue.bVal = std::stoi(szValue);
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_LONG:
		::VariantInit(&vValue);
		vValue.vt = VT_INT;
		vValue.intVal = std::stoi(szValue);
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_SHORT:
	case SQL_C_TINYINT:
		::VariantInit(&vValue);
		vValue.vt = VT_INT;
		vValue.iVal = std::stoi(szValue);
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_UBIGINT:
		::VariantInit(&vValue);
		vValue.vt = VT_INT;
		vValue.llVal = std::stoll(szValue);
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_INTERVAL_SECOND:
		::VariantInit(&vValue);
		vValue.vt = VT_UI8;
		vValue.ullVal = std::stoull(szValue);
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_TYPE_TIMESTAMP:
	case SQL_C_TYPE_TIME:
	case SQL_C_TYPE_DATE:
		::VariantInit(&vValue);
		vValue.vt = VT_UI8;
		vValue.ullVal = std::stoull(szValue);
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	default:
		break;
	}
}

