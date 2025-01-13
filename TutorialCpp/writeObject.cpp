#include "pch.h"

void writeObject(WInventory* pInventory)
{
	// create object
	WResponsible* pResponsible;
	WResponsible::Create(&pResponsible, pInventory->GetObject());

	// FullName
	wprintf(L"Full name:\n");
	std::wstring input1;
	getline(std::wcin, input1);
	pResponsible->SetFullName(input1.c_str());

	// Comment
	wprintf(L"Comment:\n");
	std::wstring input2;
	getline(std::wcin, input2);
	pResponsible->SetComment(input2.c_str());

	// set objectlink
	pInventory->LinkManager(pResponsible);

	// Store
	pResponsible->Store();	// new object
	pInventory->Store();

	// Execute
	HRESULT hRes;
	if(FAILED(hRes = pInventory->GetDomain()->Execute(Transaction::Store, NULL)))
	{
		wprintf(L"Domain failed to execute the transaction (0x%x)", hRes);
	}

	pResponsible->Release();
}
