#include "DataSQLServerAccessAssembly.h"

int APIENTRY DllMain(_In_ HINSTANCE hInstance, DWORD fdwReason, PVOID fImpLoad)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DrvFtaeAlarm::DataSQLServerAccessAssembly::instance().ClearAssemblies();
		break;
	default:
		break;
	}
	return (TRUE);
}

