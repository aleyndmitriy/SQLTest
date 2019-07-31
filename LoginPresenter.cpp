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
	_database->loadServerInstances("SQL Server Native Client 11.0");
	_settingsDataSource->Load(attributes);
	std::shared_ptr< ILoginViewInput> ptrView = view.lock();
	if (ptrView) {
		ptrView->LoadConnectionSettings(attributes);
	}
}

void DrvFtaeAlarm::LoginPresenter::FindServerList()
{
	std::shared_ptr< ILoginViewInput> ptrView = view.lock();
	serverNames.clear();
	if (ptrView) {
		serverNames = _database->GetServersList();
		ptrView->LoadServerList(serverNames);
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

void DrvFtaeAlarm::LoginPresenter::ConnectToServer()
{
	if (!attributes.serverName.empty() && !attributes.loginName.empty() && !attributes.password.empty()) {
		_database->loadDatabaseInstances(attributes.serverName, DrvFtaeAlarm::DatabaseEngine::AuthenticationType::Server, attributes.loginName, attributes.password);
		std::shared_ptr<ILoginViewInput> ptrView = view.lock();
		if (ptrView) {
			databaseNames = _database->GetDatabasesList();
			ptrView->LoadDatabasesList(databaseNames);
		}
	}
}

void DrvFtaeAlarm::LoginPresenter::ConnectToDatabase()
{
	if (!attributes.databaseName.empty()) {
		bool isOpened = _database->ChooseDatabase(attributes.databaseName);
		
	}
}

void  DrvFtaeAlarm::LoginPresenter::SetConnection()
{
	if (!attributes.serverName.empty() && !attributes.loginName.empty() && !attributes.password.empty()) {
		_database->loadDatabaseInstances(attributes.serverName, DrvFtaeAlarm::DatabaseEngine::AuthenticationType::Server, attributes.loginName, attributes.password);
		std::shared_ptr<ILoginViewInput> ptrView = view.lock();
		if (ptrView) {
			databaseNames = _database->GetDatabasesList();
			if (attributes.databaseName.empty()) {
				ptrView->LoadDatabasesList(databaseNames);
			}
			else {
				std::vector<std::string>::const_iterator itr = std::find(databaseNames.cbegin(), databaseNames.cend(), attributes.databaseName);
				if (itr != databaseNames.cend()) {
					bool isOpened = _database->ChooseDatabase(attributes.databaseName);
				}
				else {
					ptrView->LoadDatabasesList(databaseNames);
				}
			}
		}
	}
}

void DrvFtaeAlarm::LoginPresenter::SaveSettings()
{
	_settingsDataSource->Save(attributes);
}