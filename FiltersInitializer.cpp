#include"FiltersInitializer.h"
#include"DataSQLServerAccessAssembly.h"

void DrvFtaeAlarm::FiltersInitializer::CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent)
{
	std::shared_ptr<IFiltersViewOutput> presenter = std::make_shared<FiltersPresenter>(DataSQLServerAccessAssembly::instance().GetDataBaseEngine(), DataSQLServerAccessAssembly::instance().GetDatabaseInfoSQLServerDao(), DataSQLServerAccessAssembly::instance().GetSettingDataSource());
	FiltersViewController* controller = new FiltersViewController(parent, presenter);
	HWND hParentHandle = NULL;
	if (parent) {
		HWND* ptrParentHandle = reinterpret_cast<HWND*>(parent->GetInterface(0));
		hParentHandle = *ptrParentHandle;
	}
	HWND window = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TABPAGE_FILTERS), hParentHandle, FiltersDlg_Proc, (LPARAM)controller);
	ShowWindow(window, SW_HIDE);
}