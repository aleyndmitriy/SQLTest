#include"FtaeServer.h"
#include"OdsErr.h"

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
	return 0;
}

int FtaeServer::DestroyResult(ODS::HdaCommandResult* pResult)
{
	return 0;
}
