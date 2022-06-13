#include "pch.h"

void writeAttributes(WSupplies* pSupplies)
{
	// Description
	std::wstring input;
	wprintf(L"Supplies description:\n");
	std::wcin >> input;
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
