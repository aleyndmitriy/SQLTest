#include"FiltersInitializer.h"

void DrvFtaeAlarm::FiltersInitializer::CreateModule(HINSTANCE hInstance, const std::shared_ptr<UIDialogViewController>& parent)
{
	std::shared_ptr<IFiltersViewOutput> presenter = std::make_shared<FiltersPresenter>();
	FiltersViewController* controller = new FiltersViewController(parent, presenter);
	HWND hParentHandle = NULL;
	if (parent) {
		HWND* ptrParentHandle = reinterpret_cast<HWND*>(parent->GetInterface());
		hParentHandle = *ptrParentHandle;
	}
	HWND window = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_TABPAGE_FILTERS), hParentHandle, FiltersDlg_Proc, (LPARAM)controller);
	ShowWindow(window, SW_HIDE);
}