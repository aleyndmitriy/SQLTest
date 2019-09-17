#include"UISettingsConfigurator.h"
#include "OdsErr.h"
#include"SettingsInitializer.h"
#include "Constants.h"
UISettingsConfigurator::UISettingsConfigurator(const std::shared_ptr<DrvFtaeAlarm::ISettingsDataSource>& settingsDataSource, std::function<ODS::UI::IAbstractUIFacrory* (void)> uiFactoryGetter):_settingsDataSource(settingsDataSource),_uiFactoryGetter(uiFactoryGetter), _hParentWindow(nullptr)
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

	int isOk = DrvFtaeAlarm::SettingsInitializer::CreateModule(GetModuleHandle("Drv_Ftae_HdaAlarm.dll"), _uiFactoryGetter,&_hParentWindow);
	
	if (isOk) {
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
			DeleteFile("OutputXML.xml");
			DeleteFile(FITERS_XML_FILE_NAME);
			DeleteFile(SETTINGS_XML_FILE_NAME);
			hFile = INVALID_HANDLE_VALUE;
			iRes = ODS::ERR::OK;
		}
		else {
			DeleteFile("OutputXML.xml");
			DeleteFile(FITERS_XML_FILE_NAME);
			DeleteFile(SETTINGS_XML_FILE_NAME);
		}
	}
	else {
		DeleteFile(FITERS_XML_FILE_NAME);
		DeleteFile(SETTINGS_XML_FILE_NAME);
		if (szCfgInString != NULL)
		{
			size_t len = _tcslen(szCfgInString);
			char* outStr = new char[len + 1];
			_tcscpy_s(outStr, len + 1, szCfgInString);
			*pszCfgOutString = outStr;
			iRes = ODS::ERR::OK;
		}
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

void* UISettingsConfigurator::GetInterface(int nIfcId)
{
	return NULL;
}

void UISettingsConfigurator::SetOwnerWnd(void* pOwnerWnd) {
	_hParentWindow = (HWND)(pOwnerWnd);
}
