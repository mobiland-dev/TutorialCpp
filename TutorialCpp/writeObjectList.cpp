#include "pch.h"

void writeObjectList(WInventory* pInventory)
{
	// create object
	WShopArticle* pShopArticle;
	WShopArticle::Create(&pShopArticle, pInventory->GetObject());

	// ArticleName
	wprintf(L"Article name:  \n");
	std::wstring input1;
	getline(std::wcin, input1);
	pShopArticle->SetArticleName(input1.c_str());

	// Count
	wprintf(L"Count: \n");
	std::wstring input2;
	getline(std::wcin, input2);
	pShopArticle->SetCount((UINT16)_wtoi(input2.c_str()));

	// insert objectlink into list
	ArticleList* pArticleList;
	pInventory->SetItems(&pArticleList);

	ArticleListItem Item;
	Item.olArticle = pShopArticle;

	pArticleList->Insert(NULL, &Item);

	// Store
	pShopArticle->Store();	// new object
	pInventory->Store();	// we added an item to the list

	// Execute
	HRESULT hRes;
	if(FAILED(hRes = pInventory->GetDomain()->Execute(Transaction::Store, NULL)))
	{
		wprintf(L"Domain failed to execute the transaction (0x%x)", hRes);
	}

	pShopArticle->Release();
}
