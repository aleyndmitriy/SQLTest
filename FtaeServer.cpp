#include"FtaeServer.h"
#include"OdsErr.h"
#include "HdaFunctionTypes.h"
#include "HdaFunctionParam.h"
#include "HdaCommand.h"
#include"HdaCommandParam.h"


FtaeServer::FtaeServer(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseEngine>& database):_settingsDataSource(settingsDataSource), _database(database), cfgString()
{

}

FtaeServer::~FtaeServer()
{
	_settingsDataSource.reset();
	_database.reset();
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
			if (!OpenConnection()) {
				return ODS::ERR::DB_CONNECTION_FAILED;
			}
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

bool FtaeServer::OpenConnection()
{
	if (!_settingsDataSource) {
		return false;
	}
	DrvFtaeAlarm::ConnectionAttributes attributes;
	_settingsDataSource->Load(attributes);
	bool isOpen = _database->OpenConnection(attributes);
	return isOpen;
}