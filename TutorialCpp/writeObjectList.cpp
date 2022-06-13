#include "pch.h"

void writeObjectList(WInventory* pInventory)
{
	// create object
	WShopArticle* pShopArticle;
	WShopArticle::Create(&pShopArticle, pInventory);

	// ArticleName
	std::wstring input1;
	wprintf(L"Article name:  \n");
	getline(std::wcin, input1);
	pShopArticle->SetArticleName(input1.c_str());

	// Count
	std::wstring input2;
	wprintf(L"Count: \n");
	getline(std::wcin, input2);
	pShopArticle->SetCount((UINT16)_wtoi(input2.c_str()));

	// insert objectlink into list
	ArticleList* pArticleList;
	pInventory->SetItems(&pArticleList);

	ArticleListItem Item;
	Item.olShopArticle = pShopArticle;

	pArticleList->Insert(NULL, &Item);

	// Store
	pShopArticle->Store();	// new object
	pInventory->Store();	// we added an item to the list

	// Execute
	HRESULT hRes;
	if(FAILED(hRes = pInventory->GetDomain()->Execute(Transaction::Store)))
	{
		wprintf(L"Domain failed to execute the transaction (0x%x)", hRes);
	}

	pShopArticle->Release();
}
