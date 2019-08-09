#include"FtaeServer.h"
#include"OdsErr.h"
#include"Property.h"
#include"AlarmFilter.h"
#include "AlarmPropertyId.h"
#include "HdaFunctionTypes.h"
#include "HdaFunctionParam.h"
#include "HdaCommand.h"
#include"HdaCommandParam.h"
#include "SQLServerType.h"
#include"Constants.h"

FtaeServer::FtaeServer(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo):_settingsDataSource(settingsDataSource), _databaseInfo(databaseInfo), cfgString()
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
			std::map<std::string, DrvFtaeAlarm::PropertyType> properties = LoadAttributes();
			if (properties.empty()) {
				return ODS::ERR::DB_NO_DATA;
			}
			for (int ind = 0; ind < nCount; ind++) {
				ODS::HdaFunctionResult* pFuncResult = NULL;
				int nFnType = pList[ind]->GetType();
				if (nFnType == ODS::HdaFunctionType::ALARM_LIST) {
					std::string sqc;
					std::vector<DrvFtaeAlarm::PRIORITY_FILTER> filterList;
					std::vector<std::string> staticFilterList;
					pFuncResult = new ODS::HdaFunctionResultAlarmList;
					if (ODS::ERR::OK == GetFuncParameterList(pList[ind], sqc, filterList, staticFilterList)) {

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


std::map<std::string, DrvFtaeAlarm::PropertyType> FtaeServer::LoadAttributes()
{
	std::map<std::string, DrvFtaeAlarm::PropertyType> properties;
	if (!_settingsDataSource) {
		return properties;
	}
	DrvFtaeAlarm::ConnectionAttributes attributes;
	if (!_settingsDataSource->Load(attributes)) {
		return properties;
	}
	std::unique_ptr<DrvFtaeAlarm::SQLTable> table = _databaseInfo->GetTableInfo(attributes, std::string(), std::string("ConditionEvent"));

	for (DrvFtaeAlarm::SQLTable::const_iterator itr = table->cbegin(); itr != table->cend(); ++itr) {
		std::pair<std::string, DrvFtaeAlarm::PropertyType> pair = std::make_pair<std::string, DrvFtaeAlarm::PropertyType>(std::string(itr->first), DrvFtaeAlarm::PropertyTypeFromString(itr->second));
		properties.insert(pair);
	}
	return properties;
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