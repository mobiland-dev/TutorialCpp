#include "pch.h"

void readObjectList(WInventory* pInventory)
{
	HRESULT hRes;

	const ArticleList* pList;
	if(S_OK == (hRes = pInventory->GetItems(&pList)))
	{
		WShopArticle** apArticle = new WShopArticle* [pList->GetLength()];

		for(UINT32 i = 0; i < pList->GetLength(); i++)
		{
			const ArticleListItem* pItem;
			pList->Get(i, &pItem);

			// Open
			WShopArticle::Open(&apArticle[i], pItem->olShopArticle, pInventory);

			// Load
			apArticle[i]->Load();
		}

		// Execute
		if(FAILED(hRes = pInventory->GetDomain()->Execute(Transaction::Load)))
		{
			wprintf(L"Domain failed to execute the transaction (0x%x)\n", hRes);
		}
		else
		{
			wprintf(L"All ArticleList items (%i)\n\n", pList->GetLength());

			for(UINT32 i = 0; i < pList->GetLength(); i++)
			{
				const wchar_t* strArticleName;
				apArticle[i]->GetArticleName(&strArticleName);

				UINT16 ui16Amount = NULL;
				apArticle[i]->GetCount(&ui16Amount);

				wprintf(L"LinkIndex: %i || Article:: %6lux %s\n", i, ui16Amount, strArticleName);
			}
		}

		for(UINT32 i = 0; i < pList->GetLength(); i++)
			apArticle[i]->Release();

		delete[] apArticle;
	}
	else
		wprintf(L"Cannot read article list (0x%x)\n", hRes);
}
