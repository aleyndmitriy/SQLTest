
#include"SettingsInitializer.h"
#include"UISettingsConfigurator.h"
#include "BrowserEvent.h"
#include "DataSQLServerAccessAssembly.h"

const TCHAR g_szAppName[] = TEXT("SQL Test");



int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	HWND hwnd = FindWindow(TEXT("#32770"), TEXT("SQL Test"));
	if (IsWindow(hwnd)) {
		return 0;
	}
	else {
		/*UISettingsConfigurator config(DrvFtaeAlarm::DataSQLServerAccessAssembly::instance().GetSettingDataSource());
		char* output = nullptr;
		char** ptrOutput = &output;
		config.Configure(NULL, ptrOutput);*/
		DrvFtaeAlarm::ConnectionAttributes attr;
		attr.driver = std::string("SQL Server Native Client 11.0");
		attr.serverName = std::string("DESKTOP-10HG5II");
		attr.loginName = std::string("admin");
		attr.password = std::string("admin");
		attr.databaseName = std::string("FTAE");
		std::shared_ptr<DrvFtaeAlarm::DatabaseInfoDAO> info = DrvFtaeAlarm::DataSQLServerAccessAssembly::instance().GetDatabaseInfoSQLServerDao();
		std::unique_ptr<DrvFtaeAlarm::SQLDatabase> table = info->GetDatabaseInfo(attr, attr.databaseName);
		return 0;
	}
}

