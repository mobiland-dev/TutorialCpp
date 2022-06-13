#include "pch.h"

void readAttributes(WSupplies* pSupplies)
{
	HRESULT hRes;

	// Description
	const wchar_t* strDescription;
	if(S_OK == (hRes = pSupplies->GetDescription(&strDescription)))
		wprintf(L"Supplies description: %s\n", strDescription);
	else
		wprintf(L"Cannot read supplies description (0x%x)\n", hRes);

	// LastUpdated 
	UINT64 ullCurrentTime;
	if(S_OK == (hRes = pSupplies->GetLastUpdated(&ullCurrentTime)))
	{
		SYSTEMTIME lastTime;
		::FileTimeToSystemTime((FILETIME*)&ullCurrentTime, &lastTime);
		wprintf(L"Time last updated: %02i.%02i.%04i %02i:%02i (UTC)\n", lastTime.wDay, lastTime.wMonth, lastTime.wYear, lastTime.wHour, lastTime.wMinute);
	}
	else
		wprintf(L"Cannot read supplies time (0x%x)\n", hRes);
}
