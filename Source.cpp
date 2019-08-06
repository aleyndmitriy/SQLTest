
#include"SettingsInitializer.h"
#include"UISettingsConfigurator.h"
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
		//UISettingsConfigurator config;
		//char* output = nullptr;
		//char** ptrOutput = &output;
		//config.Configure(NULL, ptrOutput);

		return 0;
	}
}

