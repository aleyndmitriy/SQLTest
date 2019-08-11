#pragma once
#include"IServerFtae.h"
#include "ISettingsDataSource.h"
#include<memory>
#include"DatabaseInfoDAO.h"
#include"ConditionRecordsDAO.h"
#include "HdaFunction.h"
#include "HdaFunctionResult.h"
#include "Property.h"

class FtaeServer final : public ODS::IServerFtae {
public:
	FtaeServer() = delete;
	FtaeServer(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo, const std::shared_ptr<DrvFtaeAlarm::ConditionRecordsDAO>& recordsInfo);
	FtaeServer(const FtaeServer& src) = delete;
	FtaeServer(FtaeServer&& src) = delete;
	FtaeServer& operator=(const FtaeServer& rhs) = delete;
	FtaeServer& operator=(FtaeServer&& rhs) = delete;
	~FtaeServer();
	int Init(TCHAR* szCfgString) override;
	int Shut() override;
	void* GetInterface() override;
	int IsHdaFunctionSupported(int nFuncType) override;
	int Execute(ODS::HdaCommand* pCommand, ODS::HdaCommandResult** ppResult) override;
	int DestroyResult(ODS::HdaCommandResult* pResult) override;
	std::vector<DrvFtaeAlarm::Record> LoadEvents(std::vector<std::string> filters);
private:
	std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> _settingsDataSource;
	std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO> _databaseInfo;
	const std::shared_ptr<DrvFtaeAlarm::ConditionRecordsDAO> _recordsInfo;
	std::string cfgString;
	int GetCmdParameterList(ODS::HdaCommand* pCommand, SYSTEMTIME& rStartTime, SYSTEMTIME& rEndTime);
	int GetFuncParameterList(ODS::HdaFunction* pFunc, std::string& szSqc, std::vector<DrvFtaeAlarm::PRIORITY_FILTER>& filterList,
		std::vector<std::string>& staticFilterList);
	int BuildFuncResult(ODS::HdaFunctionResult* pFuncResult, const std::vector<DrvFtaeAlarm::Record>& rRecordList);
};

USHORT VariantToUSHORT(VARIANT* pvValue);
void SetODSProperty(ODS::Property& prop, ULONG ulId, const TCHAR* szName, const std::string& szValue, short type);