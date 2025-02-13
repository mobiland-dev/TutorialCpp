#include "pch.h"

void readObject(WInventory* pInventory)
{
	HRESULT hRes;

	// Open
	WResponsible* pResponsible;
	if(S_OK == (hRes = pInventory->OpenManager(&pResponsible, 0)))
	{
		// Load
		pResponsible->Load();

		// Execute
		if(FAILED(hRes = pResponsible->GetDomain()->Execute(Transaction::Load, NULL)))
		{
			wprintf(L"Domain failed to execute the transaction (0x%x)\n", hRes);
		}
		else
		{
			// FullName
			const wchar_t* strFullName;
			pResponsible->GetFullName(&strFullName);
			wprintf(L"Responsible full name: %s\n", strFullName);

			// Comment
			const wchar_t* strComment;
			pResponsible->GetComment(&strComment);
			wprintf(L"Responsible comment: %s\n", strComment);
		}

		pResponsible->Release();
	}
	else
		wprintf(L"Cannot open responsible (0x%x)\n", hRes);
}
