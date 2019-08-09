#pragma once
#include"IServerFtae.h"
#include "ISettingsDataSource.h"
#include<memory>
#include"DatabaseInfoDAO.h"
#include "HdaFunction.h"
#include "HdaFunctionResult.h"

class FtaeServer final : public ODS::IServerFtae {
public:
	FtaeServer() = delete;
	FtaeServer(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, const std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO>& databaseInfo);
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
private:
	std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource> _settingsDataSource;
	std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO> _databaseInfo;
	std::string cfgString;
	int GetCmdParameterList(ODS::HdaCommand* pCommand, SYSTEMTIME& rStartTime, SYSTEMTIME& rEndTime);
	int GetFuncParameterList(ODS::HdaFunction* pFunc, std::string& szSqc, std::vector<DrvFtaeAlarm::PRIORITY_FILTER>& filterList,
		std::vector<std::string>& staticFilterList);
	std::map<std::string, DrvFtaeAlarm::PropertyType> LoadAttributes();
};

USHORT VariantToUSHORT(VARIANT* pvValue);