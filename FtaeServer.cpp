#include"FtaeServer.h"
#include"OdsErr.h"
#include "Alarm.h"
#include "TimeUtils.h"
#include"AlarmFilter.h"
#include "AlarmPropertyId.h"
#include "HdaFunctionTypes.h"
#include "HdaFunctionParam.h"
#include "HdaCommand.h"
#include"HdaCommandParam.h"
#include "SQLServerType.h"
#include"Constants.h"
#include"StatementCondition.h"
#include"Log.h"
#include<algorithm>

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
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Server Init, %s"), (0 == szCfgString) ? _T("") : szCfgString);
	return iRes;
}

int  FtaeServer::Shut()
{
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Server Shut"));
	return ODS::ERR::OK;
}

void* FtaeServer::GetInterface(int nIfcId)
{
	return NULL;
}

int FtaeServer::IsHdaFunctionSupported(int nFuncType)
{
	if (nFuncType == ODS::HdaFunctionType::ALARM_LIST)
	{
		DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Server HDA functions supported"));
		return 1;
	}
	return 0;
}

int FtaeServer::Execute(ODS::HdaCommand* pCommand, ODS::HdaCommandResult** ppResult)
{
	int iRes = ODS::ERR::OK;

	if (!pCommand || !ppResult)
		return ODS::ERR::BAD_PARAM;
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Server executing commands..."));
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
						std::vector<DrvFtaeAlarm::Record> records = LoadEvents(staticFilterList, filterList,stTimeFrom,stTimeTo,sqc);
						DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("Server has loaded events..."));
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
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetCmdParameterList"));
	ODS::HdaCommandParam** ppCmdParamList = NULL;
	int nCount = 0;

	if (!pCommand)
		return ODS::ERR::BAD_PARAM;

	iRes = pCommand->GetParamList(&ppCmdParamList, &nCount);
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetCmdParameterList: Number of parameters: %d"), nCount);
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
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetFuncParameterList"));

	iRes = pFunc->GetParameterList(&ppFPList, &nCountParam);
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("GetFuncParameterList: Number of parameters: %d"), nCountParam);
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
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("BuildFuncResult"));

	if (rRecordList.empty()) {
		return ODS::ERR::DB_NO_DATA;
	}
	size_t len = cfgString.size();
	if (_settingsDataSource) {
		_settingsDataSource->LoadSettingsString(cfgString.c_str(), len + 1);
	}
	DrvFtaeAlarm::ConnectionAttributes attributes;
	if (!_settingsDataSource->Load(attributes)) {
		return  ODS::ERR::DB_NO_DATA;
	}

	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("BuildFuncResult: Number of parameters: %d"), rRecordList.size());
	if (attributes.isAlarmReport) {
		return BuildFuncAlarmsResult(pFuncResult, rRecordList);
	}
	else {
		return BuildFuncEventsResult(pFuncResult, rRecordList);
	}
}

int FtaeServer::BuildFuncAlarmsResult(ODS::HdaFunctionResult* pFuncResult, const std::vector<DrvFtaeAlarm::Record>& rRecordList)
{
	std::map<std::string, std::vector<DrvFtaeAlarm::Record> > alarmMap;

	for (std::vector<DrvFtaeAlarm::Record>::const_iterator recordsIterator = rRecordList.cbegin(); recordsIterator != rRecordList.cend(); ++recordsIterator) {
		DrvFtaeAlarm::Record::const_iterator itr = recordsIterator->findColumnValue(std::string("EventAssociationID"));
		if (itr != recordsIterator->cend()) {
			std::map<std::string, std::vector<DrvFtaeAlarm::Record> >::iterator alarmItr = alarmMap.find(itr->second.second);
			if (alarmItr != alarmMap.end()) {
				alarmItr->second.push_back(*recordsIterator);
				std::sort(alarmItr->second.begin(), alarmItr->second.end(), [](const DrvFtaeAlarm::Record& rec1, const DrvFtaeAlarm::Record& rec2) {
					DrvFtaeAlarm::Record::const_iterator itr1 = rec1.findColumnValue(std::string("TicksTimeStamp"));
					DrvFtaeAlarm::Record::const_iterator itr2 = rec2.findColumnValue(std::string("TicksTimeStamp"));
					if (itr1 == rec1.cend()) {
						return false;
					}
					if (itr2 == rec2.cend()) {
						return true;
					}
					return (itr1->second.second < itr2->second.second);
					});
			}
			else {
				std::pair<std::string, std::vector<DrvFtaeAlarm::Record> > pairMap =
					std::make_pair<std::string, std::vector<DrvFtaeAlarm::Record> >(std::string(itr->second.second), std::vector<DrvFtaeAlarm::Record>{*recordsIterator});
				alarmMap.insert(pairMap);
			}
		}
	}
	if (alarmMap.empty()) {
		return ODS::ERR::DB_NO_DATA;
	}
	int iAddRecCount = 0;
	ODS::Alarm* pAlarmList = new ODS::Alarm[alarmMap.size()];
	if (!pAlarmList) {
		return ODS::ERR::MEMORY_ALLOCATION_ERR;
	}
	ODS::HdaFunctionResultAlarmList* pFR = (ODS::HdaFunctionResultAlarmList*)pFuncResult;
	for (std::map<std::string, std::vector<DrvFtaeAlarm::Record> >::const_iterator alarmItr = alarmMap.cbegin(); alarmItr != alarmMap.cend(); ++alarmItr) {
		std::vector<ODS::Property> listProp;
		for (std::vector<DrvFtaeAlarm::Record>::const_iterator finditr = alarmItr->second.cbegin(); finditr != alarmItr->second.cend(); ++finditr) {
			bool isActiveFind = false;
			bool isInActiveFind = false;
			bool isInAckedFind = false;
			DrvFtaeAlarm::Record::const_iterator itr = finditr->findColumnValue(std::string("Active"));
			if (itr != finditr->cend()) {
				if (itr->second.second == std::string("1") && isActiveFind == false) {
					unsigned int index = 0;
					isActiveFind = true;
					for (DrvFtaeAlarm::Record::const_iterator columnIter = finditr->cbegin(); columnIter != finditr->cend(); ++columnIter) {
						ODS::Property prop;
						SetODSProperty(prop, PROP_START_ID + index, columnIter->first.c_str(), columnIter->second.second, columnIter->second.first);
						++index;
						listProp.push_back(prop);
						if (columnIter->first == std::string("EventTimeStamp")) {
							ODS::Property prop;
							SetODSTimeProperty(prop, ODS::AlarmProperty::ID_START_TIME, TEXT("StartTime"), columnIter->second.second);
							listProp.push_back(prop);
						}
						if (columnIter->first == std::string("Priority")) {
							ODS::Property prop;
							SetODSProperty(prop, ODS::AlarmProperty::ID_PRIORITY, TEXT("Priority"), columnIter->second.second, columnIter->second.first);
							listProp.push_back(prop);
						}
						if (columnIter->first == std::string("Message")) {
							ODS::Property prop;
							SetODSProperty(prop, ODS::AlarmProperty::ID_TEXT, TEXT("Description"), columnIter->second.second, columnIter->second.first);
							listProp.push_back(prop);
						}
					}
				}
				if (itr->second.second == std::string("0") && isInActiveFind == false) {
					isInActiveFind = true;
					DrvFtaeAlarm::Record::const_iterator itrEndTime = finditr->findColumnValue(std::string("EventTimeStamp"));
					if (itrEndTime != finditr->cend()) {
						ODS::Property prop;
						SetODSTimeProperty(prop, ODS::AlarmProperty::ID_END_TIME, TEXT("EndTime"), itrEndTime->second.second);
						listProp.push_back(prop);
					}
				}
			}
			DrvFtaeAlarm::Record::const_iterator itrAcked = finditr->findColumnValue(std::string("Acked"));
			if (itrAcked != finditr->cend()) {
				if (itrAcked->second.second == std::string("1") && isInAckedFind == false) {
					isInAckedFind = true;
					DrvFtaeAlarm::Record::const_iterator itrAckedTime = finditr->findColumnValue(std::string("EventTimeStamp"));
					if (itrAckedTime != finditr->cend()) {
						ODS::Property prop;
						SetODSTimeProperty(prop, ODS::AlarmProperty::ID_ACK_TIME, TEXT("AckTime"), itrAckedTime->second.second);
						listProp.push_back(prop);
					}
				}
			}
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

int FtaeServer::BuildFuncEventsResult(ODS::HdaFunctionResult* pFuncResult, const std::vector<DrvFtaeAlarm::Record>& rRecordList)
{
	int iAddRecCount = 0;
	ODS::Alarm* pAlarmList = new ODS::Alarm[rRecordList.size()];
	if (!pAlarmList) {
		return ODS::ERR::MEMORY_ALLOCATION_ERR;
	}
	ODS::HdaFunctionResultAlarmList* pFR = (ODS::HdaFunctionResultAlarmList*)pFuncResult;
	for (std::vector<DrvFtaeAlarm::Record>::const_iterator recordsIterator = rRecordList.cbegin(); recordsIterator != rRecordList.cend(); ++recordsIterator) {
		std::vector<ODS::Property> listProp;
		unsigned int index = 0;
		for (DrvFtaeAlarm::Record::const_iterator columnIter = recordsIterator->cbegin(); columnIter != recordsIterator->cend(); ++columnIter) {
			ODS::Property prop;
			SetODSProperty(prop, PROP_START_ID + index, columnIter->first.c_str(), columnIter->second.second, columnIter->second.first);
			++index;
			listProp.push_back(prop);
			if (columnIter->first == std::string("EventTimeStamp")) {
				DrvFtaeAlarm::Record::const_iterator itr = recordsIterator->findColumnValue(std::string("Active"));
				if (itr != recordsIterator->cend()) {
					if (itr->second.second == std::string("1")) {
						ODS::Property prop;
						SetODSTimeProperty(prop, ODS::AlarmProperty::ID_START_TIME, TEXT("StartTime"), columnIter->second.second);
						listProp.push_back(prop);
					}
					if (itr->second.second == std::string("0")) {
						ODS::Property prop;
						SetODSTimeProperty(prop, ODS::AlarmProperty::ID_END_TIME, TEXT("EndTime"), columnIter->second.second);
						listProp.push_back(prop);
					}
				}
				DrvFtaeAlarm::Record::const_iterator itrAcked = recordsIterator->findColumnValue(std::string("Acked"));
				if (itrAcked != recordsIterator->cend())
				{
					if (itrAcked->second.second == std::string("1")) {
						ODS::Property prop;
						SetODSTimeProperty(prop, ODS::AlarmProperty::ID_ACK_TIME, TEXT("AckTime"), columnIter->second.second);
						listProp.push_back(prop);
					}
				}
			}
			if (columnIter->first == std::string("Priority")) {
				ODS::Property prop;
				SetODSProperty(prop, ODS::AlarmProperty::ID_PRIORITY, TEXT("Priority"), columnIter->second.second, columnIter->second.first);
				listProp.push_back(prop);
			}
			if (columnIter->first == std::string("Message")) {
				ODS::Property prop;
				SetODSProperty(prop, ODS::AlarmProperty::ID_TEXT, TEXT("Description"), columnIter->second.second, columnIter->second.first);
				listProp.push_back(prop);
			}
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

std::vector<DrvFtaeAlarm::Record> FtaeServer::LoadEvents(const std::vector<std::string>& filters, const std::vector<DrvFtaeAlarm::PRIORITY_FILTER>& priorityFilters, const SYSTEMTIME& timeStart, const SYSTEMTIME& timeFinish, const std::string& sqlCondition)
{
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("LoadEvents"));
	
	std::vector<DrvFtaeAlarm::Record> records;
	if (!_settingsDataSource) {
		return records;
	}
	size_t len = cfgString.size();
	if (_settingsDataSource) {
		_settingsDataSource->LoadSettingsString(cfgString.c_str(), len + 1);
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
	std::unique_ptr<DrvFtaeAlarm::SQLTable> table = _databaseInfo->GetTableInfo(attributes, attributes.databaseName, std::string("ConditionEvent"));
	if (!table) {
		return records;
	}
	for (DrvFtaeAlarm::SQLTable::const_iterator itr = table->cbegin(); itr != table->cend(); ++itr) {
		if (itr->first == std::string("EventTimeStamp")) {
			conditions.push_back(StatementFromTimeStamp(itr->first,timeStart,timeFinish));
		}
		if (itr->first == std::string("Priority")) {
			for (std::vector<DrvFtaeAlarm::PRIORITY_FILTER>::const_iterator priorItr = priorityFilters.cbegin(); priorItr != priorityFilters.cend(); ++priorItr) {
				conditions.push_back(StatementFromPriority(itr->first, *priorItr));
			}
		}
	}
	records = _recordsInfo->GetRecords(*table, attributes, conditions, sqlCondition);
	DrvFtaeAlarm::Log::GetInstance()->WriteInfo(_T("LoadEvents: Number of records: %d"), records.size());
	return records;
}

DrvFtaeAlarm::StatementCondition FtaeServer::StatementFromTimeStamp(const std::string& property, const SYSTEMTIME& timeStart, const SYSTEMTIME& timeFinish)
{
	SYSTEMTIME startTimeUTC;
	ODS::TimeUtils::SysTimeLocalToUtc(timeStart, &startTimeUTC);
	SYSTEMTIME endTimeUTC;
	ODS::TimeUtils::SysTimeLocalToUtc(timeFinish, &endTimeUTC);
	std::string startDate = std::to_string(startTimeUTC.wYear) + std::string("-") + std::to_string(startTimeUTC.wMonth) + std::string("-") +
		std::to_string(startTimeUTC.wDay) + std::string(" ") + std::to_string(startTimeUTC.wHour) + std::string(":") +
		std::to_string(startTimeUTC.wMinute) + std::string(":") + std::to_string(startTimeUTC.wSecond);
	std::string endDate = std::to_string(endTimeUTC.wYear) + std::string("-") + std::to_string(endTimeUTC.wMonth) + std::string("-") +
		std::to_string(endTimeUTC.wDay) + std::string(" ") + std::to_string(endTimeUTC.wHour) + std::string(":") +
		std::to_string(endTimeUTC.wMinute) + std::string(":") + std::to_string(endTimeUTC.wSecond);
	DrvFtaeAlarm::StatementCondition condition(DrvFtaeAlarm::CombineOperation::COMBINEOP_AND, property, DrvFtaeAlarm::PropertyType::PROPTYPE_DATE, DrvFtaeAlarm::ConditionType::CONDTYPE_BETWEEN, startDate,endDate);
	return condition;
}

DrvFtaeAlarm::StatementCondition FtaeServer::StatementFromPriority(const std::string& property, DrvFtaeAlarm::PRIORITY_FILTER priority)
{
	DrvFtaeAlarm::StatementCondition condition(DrvFtaeAlarm::CombineOperation::COMBINEOP_AND, property, DrvFtaeAlarm::PropertyType::PROPTYPE_NUMERIC, DrvFtaeAlarm::ConditionType::CONDTYPE_BETWEEN, std::to_string(priority.m_PriorityFrom), std::to_string(priority.m_PriorityTo));
	return condition;
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
	std::string str;
	const TIMESTAMP_STRUCT* timeStampStruct = nullptr;
	const TIME_STRUCT* timeStruct = nullptr;
	const DATE_STRUCT* dayStruct = nullptr;
	float val = 0.0;
	bool bitVal = false;
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
		vValue.dblVal = val;
		prop.SetVarValue(&vValue);
		::VariantClear(&vValue);
		break;
	case SQL_C_BIT:
		::VariantInit(&vValue);
		vValue.vt = VT_BOOL;
		if (std::stoi(szValue) > 0) {
			bitVal = true;
		}
		else {
			bitVal = false;
		}
		vValue.boolVal = bitVal;
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
		timeStampStruct = reinterpret_cast<const TIMESTAMP_STRUCT*>(szValue.c_str());
		str = std::to_string(timeStampStruct->year) + std::string("-") + std::to_string(timeStampStruct->month) + std::string("-") +
			std::to_string(timeStampStruct->day) + std::string(" ") + std::to_string(timeStampStruct->hour) + std::string(":") + std::to_string(timeStampStruct->minute) +
			std::string(":") + std::to_string(timeStampStruct->second) + std::string(".") + std::to_string(timeStampStruct->fraction);
		prop.SetStrValue(str.c_str());
		break;
	case SQL_C_TYPE_TIME:
		timeStruct = reinterpret_cast<const TIME_STRUCT*>(szValue.c_str());
		str = std::to_string(timeStruct->hour) + std::string(":") + std::to_string(timeStruct->minute) +
			std::string(":") + std::to_string(timeStruct->second);
		prop.SetStrValue(str.c_str());
	case SQL_C_TYPE_DATE:
		dayStruct = reinterpret_cast<const DATE_STRUCT*>(szValue.c_str());
		str = std::to_string(dayStruct->year) + std::string("-") + std::to_string(dayStruct->month) + std::string("-") +
			std::to_string(dayStruct->day);
		prop.SetStrValue(str.c_str());
		break;
	default:
		break;
	}
}

void SetODSTimeProperty(ODS::Property& prop, ULONG ulId, const TCHAR* szName, const std::string& szValue)
{
	VARIANT vValue;
	ULONG64 ul64Millisec = 0;
	SYSTEMTIME dataTime = { 0 };
	const TIMESTAMP_STRUCT* timeStampStruct = reinterpret_cast<const TIMESTAMP_STRUCT*>(szValue.c_str());
	prop.SetFlag(ODS::Property::PROP_FLAG_ACCESS_READ_ONLY, true);
	prop.SetId(ulId);
	prop.SetName(szName);
	::VariantInit(&vValue);
	vValue.vt = VT_UI8;
	dataTime.wYear = timeStampStruct->year;
	dataTime.wMonth = timeStampStruct->month;
	dataTime.wDay = timeStampStruct->day;
	dataTime.wHour = timeStampStruct->hour;
	dataTime.wMinute = timeStampStruct->minute;
	dataTime.wSecond = timeStampStruct->second;
	dataTime.wMilliseconds = timeStampStruct->fraction;
	ODS::TimeUtils::SysTimeLocalToUlong64(dataTime, &ul64Millisec);
	vValue.ullVal = ul64Millisec;
	prop.SetVarValue(&vValue);
	::VariantClear(&vValue);
}

