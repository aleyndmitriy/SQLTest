#include"LoginPresenter.h"
#include "SQLServerConnection.h"
DrvFtaeAlarm::LoginPresenter::LoginPresenter(const std::shared_ptr<DatabaseEngine> &database, const std::shared_ptr<ISettingsDataSource>& settingsDataSource):view(),_database(database),_settingsDataSource(settingsDataSource)
{

}

DrvFtaeAlarm::LoginPresenter::~LoginPresenter() 
{
	view.reset();
	_database.reset();
	_settingsDataSource.reset();
}

void DrvFtaeAlarm::LoginPresenter::SetViewInput(std::shared_ptr<ILoginViewInput> input)
{
	view = input;
}

void DrvFtaeAlarm::LoginPresenter::viewIsReady()
{
	_database->OpenConnection();
	_settingsDataSource->Load(attributes);
	std::shared_ptr< ILoginViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->LoadConnectionSettings(attributes);
	}
}

void DrvFtaeAlarm::LoginPresenter::GetServerName(std::string&& serverName)
{
	attributes.serverName = std::move(serverName);
}

void DrvFtaeAlarm::LoginPresenter::GetLogin(std::string&& login)
{
	attributes.loginName = std::move(login);
}


void DrvFtaeAlarm::LoginPresenter::GetPassword(std::string&& password)
{
	attributes.password = std::move(password);
}

void DrvFtaeAlarm::LoginPresenter::GetDatabaseName(std::string&& databaseName)
{
	attributes.databaseName = std::move(databaseName);
}

void DrvFtaeAlarm::LoginPresenter::GetAuthType(int isSystem) {
	attributes.isServerAuthentication = (isSystem > 0);
}

void DrvFtaeAlarm::LoginPresenter::GetServerIndex(int index)
{
	if (!serverNames.empty() && index < serverNames.size()) {
		attributes.serverName = serverNames.at(index);
	}
}

void DrvFtaeAlarm::LoginPresenter::GetDatabaseIndex(int index)
{
	if (!databaseNames.empty() && index < databaseNames.size()) {
		attributes.databaseName = databaseNames.at(index);
	}
}

void DrvFtaeAlarm::LoginPresenter::ConnectToDriver() {
	std::shared_ptr< ILoginViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		CloseExistentConnection();
		attributes.driver = std::string("SQL Server Native Client 11.0");
		ptrView->LoadConnectionSettings(attributes);
		if (_database->OpenConnectionIfNeeded(attributes)) {
			serverNames = _database->GetServersList();
			ptrView->LoadServerList(serverNames);
			ptrView->StopLoading();
		}
		else {
			attributes.driver.clear();
			_database->CloseConnection();
			ptrView->StopLoading();
			ptrView->ErrorMessage(std::string("Connection Error"));
		}
	}
	
}

void DrvFtaeAlarm::LoginPresenter::ConnectToServer()
{
	std::shared_ptr< ILoginViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		//attributes.databaseName.clear();
		if (_database->OpenConnectionIfNeeded(attributes)) {
			databaseNames = _database->GetDatabasesList();
			ptrView->LoadDatabasesList(databaseNames);
		}
		ptrView->StopLoading();
	}
	
}

void DrvFtaeAlarm::LoginPresenter::CheckConnectToDatabase()
{
	std::shared_ptr< ILoginViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->StartLoading();
		if (attributes.driver.empty()) {
			attributes.driver = std::string("SQL Server Native Client 11.0");
		}
		if (!attributes.serverName.empty()) {
			if (!_database->OpenConnectionIfNeeded(attributes)) {
				ptrView->StopLoading();
				ptrView->ErrorMessage(std::string("Connection Test Failed!"));
			}
			else {
				databaseNames = _database->GetDatabasesList();
				ptrView->LoadDatabasesList(databaseNames);
				if (!attributes.databaseName.empty()) {
					ptrView->SelectDatabase(attributes.databaseName);
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

void DrvFtaeAlarm::LoginPresenter::CloseExistentConnection()
{
	_database->CloseConnection();
	attributes.serverName.clear();
	attributes.databaseName.clear();
	attributes.loginName.clear();
	attributes.password.clear();
	attributes.driver.clear();
	attributes.isServerAuthentication = false;
	serverNames.clear();
	databaseNames.clear();
	std::shared_ptr< ILoginViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->LoadServerList(serverNames);
		ptrView->LoadDatabasesList(databaseNames);
	}
}

void DrvFtaeAlarm::LoginPresenter::SaveSettings()
{
	if (!attributes.driver.empty() && !attributes.serverName.empty() && !attributes.databaseName.empty()) {
		_settingsDataSource->Save(attributes);
	}
}

void DrvFtaeAlarm::LoginPresenter::GetReportType(bool isAlarm)
{
	attributes.isAlarmReport = isAlarm;
}