#pragma once
#include<string>
#include"IBrowserEvent.h"
#include"PropertyInfo.h"
#include "XMLSettingsDataSource.h"

class BrowserEvent final : public ODS::IBrowserEvent, public DrvFtaeAlarm::XMLSettingsDataSource
{
public:
	BrowserEvent();
	BrowserEvent(const BrowserEvent& src) = delete;
	BrowserEvent(BrowserEvent&& src) = delete;
	BrowserEvent& operator=(const BrowserEvent& rhs) = delete;
	BrowserEvent& operator=(BrowserEvent&& rhs) = delete;
	~BrowserEvent();
	void* GetInterface() override;
	int Init(TCHAR* szCfgString) override;
	int Shut() override;
	int GetExternalFilterList(TCHAR*** ppszFilterList, ULONG* pulCount) override;
	int GetFilterList(TCHAR*** ppszFilterList, ULONG* pulCount) override;
	int DestroyFilterList(TCHAR** ppFilterList, ULONG ulCount) override;
	int GetAlarmPropertyInfoList(ODS::PropertyInfo** ppPropertyInfoList, ULONG* pulCount) override;
	int DestroyAlarmPropertyInfoList(ODS::PropertyInfo* pPropertyInfoList, ULONG ulCount) override;
private:
	std::string cfgString;
};