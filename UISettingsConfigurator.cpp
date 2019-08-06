#include"UISettingsConfigurator.h"
#include "OdsErr.h"
#include"SettingsInitializer.h"

UISettingsConfigurator::UISettingsConfigurator(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource):_settingsDataSource(settingsDataSource)
{

}

UISettingsConfigurator::~UISettingsConfigurator() 
{
	_settingsDataSource.reset();
}

int UISettingsConfigurator::Configure(const TCHAR* szCfgInString, TCHAR** pszCfgOutString)
{
	int iRes = ODS::ERR::FILE;

	if (szCfgInString != NULL)
	{
		size_t len = _tcslen(szCfgInString);
		if (_settingsDataSource) {
			_settingsDataSource->LoadSettingsString(szCfgInString, len);
		}
	}

	DrvFtaeAlarm::SettingsInitializer::CreateModule(GetModuleHandle(NULL), std::shared_ptr<DrvFtaeAlarm::UIDialogViewController>());
	if (_settingsDataSource) {
		_settingsDataSource->SaveSettingsString("OutputXML.xml");
	}
	
	HANDLE hFile = CreateFile("OutputXML.xml", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD fileSizeHight = 0;
		DWORD fileSizeLow = GetFileSize(hFile, &fileSizeHight);
		if (fileSizeLow == INVALID_FILE_SIZE && fileSizeHight == NULL)
		{
			CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return iRes;
		}
		char* outStr = new char[fileSizeLow + 1];
		DWORD numRead = 0;
		BOOL isRead = FALSE;
		isRead = ReadFile(hFile, outStr, fileSizeLow, &numRead, NULL);
		if (isRead == FALSE || numRead != fileSizeLow)
		{
			CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			return ODS::ERR::FILE;
		}
		*pszCfgOutString = outStr;
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	return iRes;
}

int UISettingsConfigurator::DestroyString(TCHAR* szCfgString)
{
	if (szCfgString)
		delete[] szCfgString;

	return ODS::ERR::OK;
}


int UISettingsConfigurator::ConfigureFilters(TCHAR* szCfgInString, TCHAR** pszCfgOutString)
{
	int iRes = ODS::ERR::OK;


	return iRes;
}

void* UISettingsConfigurator::GetInterface()
{
	return NULL;
}
