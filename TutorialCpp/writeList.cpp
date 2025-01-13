#include "pch.h"

void writeList(WSupplies* pSupplies)
{
	// Shops
	ShopList* pList;
	pSupplies->SetShops(&pList);

	ShopListItem Item;

	// ShopName
	wprintf(L"Shop name:\n");
	std::wstring input1;
	getline(std::wcin, input1);
	Item.ShopName = input1.c_str();

	// ShopURL
	wprintf(L"Shop URL:\n");
	std::wstring input2;
	getline(std::wcin, input2);
	Item.ShopURL = input2.c_str();

	pList->Insert(NULL, &Item);

	// Store
	pSupplies->Store();	// we added an item to the list

	// Execute
	HRESULT hRes;
	if(FAILED(hRes = pSupplies->GetDomain()->Execute(Transaction::Store, NULL)))
	{
		wprintf(L"Domain failed to execute the transaction (0x%x)", hRes);
	}
}
