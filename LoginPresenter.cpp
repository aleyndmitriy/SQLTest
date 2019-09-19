#include"LoginPresenter.h"
#include "SQLServerConnection.h"
DrvFtaeAlarm::LoginPresenter::LoginPresenter(const std::shared_ptr<DatabaseEngine> &database, std::shared_ptr<ConnectionAttributes> externAttributes):_view(),_database(database),_externAttributes(externAttributes)
{

}

DrvFtaeAlarm::LoginPresenter::~LoginPresenter() 
{
	_view.reset();
	_database.reset();
	_externAttributes.reset();
}

void DrvFtaeAlarm::LoginPresenter::SetViewInput(std::shared_ptr<ILoginViewInput> input)
{
	_view = input;
}

void DrvFtaeAlarm::LoginPresenter::viewIsReady()
{
	_database->OpenConnection();
	std::shared_ptr< ILoginViewInput> ptrView = _view.lock();
	if (ptrView) {
		ptrView->LoadConnectionSettings(*_externAttributes);
	}
}

void DrvFtaeAlarm::LoginPresenter::GetServerName(std::string&& serverName)
{
	_externAttributes->serverName = std::move(serverName);
}

void DrvFtaeAlarm::LoginPresenter::GetLogin(std::string&& login)
{
	_externAttributes->loginName = std::move(login);
}


void DrvFtaeAlarm::LoginPresenter::GetPassword(std::string&& password)
{
	_externAttributes->password = std::move(password);
}

void DrvFtaeAlarm::LoginPresenter::GetDatabaseName(std::string&& databaseName)
{
	_externAttributes->databaseName = std::move(databaseName);
}

void DrvFtaeAlarm::LoginPresenter::GetAuthType(int isSystem) {
	_externAttributes->isServerAuthentication = (isSystem > 0);
}



void DrvFtaeAlarm::LoginPresenter::GetDatabaseIndex(int index)
{
	if (!databaseNames.empty() && index < databaseNames.size()) {
		_externAttributes->databaseName = databaseNames.at(index);
	}
}

void DrvFtaeAlarm::LoginPresenter::ConnectToDriver() {
	std::shared_ptr< ILoginViewInput> ptrView = _view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		_database->CloseConnection();
		_externAttributes->driver = std::string("SQL Server Native Client 11.0");
		ptrView->LoadConnectionSettings(*_externAttributes);
		if (_database->OpenConnectionIfNeeded(*_externAttributes)) {
			serverNames = _database->GetServersList();
			ptrView->StopLoading();
		}
		else {
			_externAttributes->driver.clear();
			_database->CloseConnection();
			ptrView->StopLoading();
			ptrView->ErrorMessage(std::string("Connection Error"));
		}
	}
	
}

void DrvFtaeAlarm::LoginPresenter::ConnectToServer()
{
	std::shared_ptr< ILoginViewInput> ptrView = _view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		//attributes.databaseName.clear();
		if (_database->OpenConnectionIfNeeded(*_externAttributes)) {
			databaseNames = _database->GetDatabasesList();
			ptrView->LoadDatabasesList(databaseNames);
		}
		ptrView->StopLoading();
	}
	
}

void DrvFtaeAlarm::LoginPresenter::CheckConnectToDatabase()
{
	std::shared_ptr< ILoginViewInput> ptrView = _view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		databaseNames.clear();
		ptrView->LoadDatabasesList(databaseNames);
		_database->CloseConnection();
		if (_externAttributes->driver.empty()) {
			_externAttributes->driver = std::string("SQL Server Native Client 11.0");
		}
		if (!_externAttributes->serverName.empty()) {
			if (!_database->OpenConnectionIfNeeded(*_externAttributes)) {
				databaseNames.clear();
				ptrView->LoadDatabasesList(databaseNames);
				ptrView->StopLoading();
				ptrView->ErrorMessage(std::string("Connection Test Failed!"));
			}
			else {
				databaseNames = _database->GetDatabasesList();
				ptrView->LoadDatabasesList(databaseNames);
				if (!_externAttributes->databaseName.empty()) {
					ptrView->SelectDatabase(_externAttributes->databaseName);
				}
				ptrView->StopLoading();
				ptrView->WarningMessage(std::string("Connection Test Succeed!"));
			}
		}
		else {
			ptrView->StopLoading();
			ptrView->ErrorMessage(std::string("Empty fields!"));
		}
	}
}



void DrvFtaeAlarm::LoginPresenter::SaveSettings()
{
	
}

void DrvFtaeAlarm::LoginPresenter::GetReportType(bool isAlarm)
{
	_externAttributes->isAlarmReport = isAlarm;
}