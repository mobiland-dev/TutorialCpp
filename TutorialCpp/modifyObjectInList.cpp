#include "pch.h"

void modifyObjectInList(WInventory* pInventory)
{
	std::wstring input0;
	wprintf(L"List index: \n");
	getline(std::wcin, input0);
	UINT32 indexInput = ((UINT32)_wtoi(input0.c_str()));

	HRESULT hRes;

	const ArticleList* pList;
	if(S_OK == (hRes = pInventory->GetItems(&pList)))
	{
		wprintf(L"Cannot read article list (0x%x)\n", hRes);
		return;
	}

	const ArticleListItem* pOldItem;
	if(S_OK != (hRes = pList->Get(indexInput, &pOldItem)))
	{
		wprintf(L"There is no article with this index\n");
		return;
	}

	WShopArticle* pArticle;
	WShopArticle::Open(&pArticle, pOldItem->olShopArticle, pInventory);

	// Load
	pArticle->Load();

	// Execute
	if(FAILED(hRes = pInventory->GetDomain()->Execute(Transaction::Load)))
	{
		wprintf(L"Domain failed to execute the transaction (0x%x)\n", hRes);
	}
	else
	{
		std::wstring input1;
		wprintf(L"Article name: \n");
		getline(std::wcin, input1);
		if(input1.length() > 0)
			pArticle->SetArticleName(input1.c_str());

		std::wstring input2;
		wprintf(L"Count: \n");
		getline(std::wcin, input2);
		if(input2.length() > 0)
			pArticle->SetCount((UINT16)_wtoi(input2.c_str()));

		// Store
		pArticle->Store();

		// Execute
		if(FAILED(hRes = pInventory->GetDomain()->Execute(Transaction::Store)))
		{
			wprintf(L"Domain failed to execute the transaction (0x%x)", hRes);
		}
	}

	pArticle->Release();
}
