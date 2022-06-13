#include "pch.h"

void readList(WSupplies* pSupplies)
{
	HRESULT hRes;

	const ShopList* pList;
	if(S_OK == (hRes = pSupplies->GetShops(&pList)))
	{
		wprintf(L"All ShopList items (%i)\n\n", pList->GetLength());

		for(UINT32 i = 0; i < pList->GetLength(); i++)
		{
			const ShopListItem* Item;
			pList->Get(i, &Item);
			wprintf(L"Shop %i\n", i);
			wprintf(L"Shop Name: %s\n", Item->ShopName);
			wprintf(L"Shop Url: %s\n\n", Item->ShopURL);
		}
	}
	else
		wprintf(L"Cannot read shop list (0x%x)\n", hRes);
}
