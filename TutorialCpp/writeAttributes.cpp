#include "pch.h"

void writeAttributes(WSupplies* pSupplies)
{
	// Description
	wprintf(L"Supplies description:\n");
	std::wstring input;
	std::getline(std::wcin, input);
	pSupplies->SetDescription(input.c_str());

	// LastUpdated
	UINT64 ullCurrentTime;
	::GetSystemTimeAsFileTime((FILETIME *)&ullCurrentTime);
	pSupplies->SetLastUpdated(ullCurrentTime);
	wprintf(L"Time was automatically set to current time.\n");

	// Store
	pSupplies->Store();

	// Execute
	HRESULT hRes;
	if(FAILED(hRes = pSupplies->GetDomain()->Execute(Transaction::Store)))
	{
		wprintf(L"Domain failed to execute the transaction (0x%x)\n", hRes);
	}
}
