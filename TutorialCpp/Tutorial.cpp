#include "pch.h"

const GUID guidEntryPoint
{ 0x71e456a5, 0x6a4e, 0x46aa, { 0xa7, 0x27, 0x10, 0x5d, 0x58, 0x4c, 0x79, 0x17 } };

HRESULT	init				(Connection*& pConnection, WDomain*& pDomain, const wchar_t* strServerAddress, UINT16 usServerPort, const GUID& guidDomainId, UINT32 ulStorageId);
void	uninit				(Connection*& pConnection, WDomain*& pDomain, UINT32 ulStorageId);

void	writeAttributes		(WSupplies* pSupplies);
void	readAttributes		(WSupplies* pSupplies);
void	writeList			(WSupplies* pSupplies);
void	readList			(WSupplies* pSupplies);
void	writeObject			(WInventory* pInventory);
void	readObject			(WInventory* pInventory);
void	writeObjectList		(WInventory* pInventory);
void	readObjectList		(WInventory* pInventory);
void	modifyObjectInList	(WInventory* pInventory);


int wmain(int argc, wchar_t* argv[])
{
	// ServerAddress, ServerPort
	const wchar_t* strServerAddress = argv[1];
	UINT16 usServerPort = (UINT16)_wtoi(argv[2]);

	// Domain-GUID
	GUID guidDomainId;
	::CLSIDFromString(argv[3], &guidDomainId);

	// Storage-ID (we are using the default 0 here)
	UINT32 ulStorageId = 0;

	Connection* pConnection;
	WDomain* pDomain;

	if(S_OK != init(pConnection, pDomain, strServerAddress, usServerPort, guidDomainId, ulStorageId))
	{
		printf("init failed\n");
		return -1;
	}

	int action;
	WSupplies* pSupplies = NULL;
	WInventory* pInventory = NULL;

	do
	{
		wprintf(L"\nselect function:\n  1: create new Supplies object\n  2: open existing Supplies object\n\n  3: create new Inventory object\n  4: extend Supplies object to Inventory object\n  5: open existing Inventory object\n");

		action = _getch();
		wprintf(L"\n");

		switch(action)
		{
		case '1':
			{
				WSupplies::Create(&pSupplies, pDomain, &DataFS::OBJECTID_NULL);

				pDomain->InsertNamedLink(&pSupplies->BuildLink(true), &guidEntryPoint, L"basic entry point");

				// Store
				pSupplies->Store();

				// Execute
				HRESULT hRes;
				if(FAILED(hRes = pDomain->Execute(Transaction::Store, NULL)))
				{
					wprintf(L"Domain failed to execute the transaction (0x%x)\n", hRes);

					pSupplies->Release();
					pSupplies = NULL;
				}
			}
			break;

		case '2':
			{
				DataFS::stcObjectLink* pSuppliesLink;
				if(S_OK == pDomain->QueryNamedLink(&guidEntryPoint, 1, &pSuppliesLink, NULL))
				{
					if(WSupplies::IsOfType(pSuppliesLink))
					{
						WSupplies::Open(&pSupplies, pDomain, *pSuppliesLink);
						pSupplies->Load();

						DataFSAccess::MemFree(pSuppliesLink);

						if(FAILED(pDomain->Execute(Transaction::Load, NULL)))
						{
							wprintf(L"Execute failed...\n");
							uninit(pConnection, pDomain, ulStorageId);
							return -1;
						}
					}
					else
					{
						DataFSAccess::MemFree(pSuppliesLink);

						wprintf(L"Object is not of type WSupplies\n");
					}
				}
				else
					wprintf(L"Entry point not found\n");
			}
			break;

		case '3':
			{
				WInventory::Create(&pInventory, pDomain, &DataFS::OBJECTID_NULL);

				pDomain->InsertNamedLink(&pInventory->BuildLink(true), &guidEntryPoint, L"extended entry point");

				pSupplies = WSupplies::CastTo(pInventory->GetObject());

				// Store
				pInventory->Store();

				// Execute
				HRESULT hRes;
				if(FAILED(hRes = pDomain->Execute(Transaction::Store, NULL)))
				{
					wprintf(L"Domain failed to execute the transaction (0x%x)\n", hRes);

					pSupplies->Release();
					pSupplies = NULL;
				}
			}
			break;

		case '4':
			{
				DataFS::stcObjectLink* pSuppliesLink;
				if(S_OK == pDomain->QueryNamedLink(&guidEntryPoint, 1, &pSuppliesLink, NULL))
				{
					if(WInventory::IsOfType(pSuppliesLink))
					{
						DataFSAccess::MemFree(pSuppliesLink);

						wprintf(L"Object is already of type WInventory\n");
					}
					else if(WSupplies::IsOfType(pSuppliesLink))
					{
						WSupplies::Open(&pSupplies, pDomain, *pSuppliesLink);
						pSupplies->Load();

						if(FAILED(pDomain->Execute(Transaction::Load, NULL)))
						{
							wprintf(L"Execute failed...\n");
							uninit(pConnection, pDomain, ulStorageId);
							return -1;
						}

						WInventory::Extend(&pInventory, pSupplies->GetObject());

						pDomain->InsertNamedLink(&pInventory->BuildLink(true), &guidEntryPoint, L"updated entry point");

						// Store
						pInventory->Store();

						// Execute
						HRESULT hRes;
						if(FAILED(hRes = pDomain->Execute(Transaction::Store, NULL)))
						{
							wprintf(L"Domain failed to execute the transaction (0x%x)\n", hRes);

							pSupplies->Release();
							pSupplies = NULL;
						}
					}
					else
					{
						DataFSAccess::MemFree(pSuppliesLink);

						wprintf(L"Object is not of type WSupplies\n");
					}
				}
				else
					wprintf(L"Entry point not found\n");
			}
			break;

		case '5':
			{
				DataFS::stcObjectLink* pInventoryLink;
				if(S_OK == pDomain->QueryNamedLink(&guidEntryPoint, 1, &pInventoryLink, NULL))
				{
					if(WInventory::IsOfType(pInventoryLink))
					{
						WInventory::Open(&pInventory, pDomain, *pInventoryLink);
						pInventory->Load();

						DataFSAccess::MemFree(pInventoryLink);

						if(FAILED(pDomain->Execute(Transaction::Load, NULL)))
						{
							wprintf(L"Execute failed...\n");
							uninit(pConnection, pDomain, ulStorageId);
							return -1;
						}

						pSupplies = WSupplies::CastTo(pInventory->GetObject());
					}
					else if(WSupplies::IsOfType(pInventoryLink))
					{
						DataFSAccess::MemFree(pInventoryLink);

						wprintf(L"Object is not of type WInventory but of type WSupplies\n");
					}
					else
					{
						DataFSAccess::MemFree(pInventoryLink);

						wprintf(L"Object is not of type WInventory\n");
					}
				}
				else
					wprintf(L"Entry point not found\n");
			}
			break;
		}

	} while(pSupplies == NULL);

	do
	{
		if(!pInventory)
			wprintf(L"\nselect function:\n  1: writeAttributes\n  2: readAttribute\n  3: writeList\n  4: readList\n  q: quit\n");
		else
			wprintf(L"\nselect function:\n  1: writeAttributes\n  2: readAttribute\n  3: writeList\n  4: readList\n  5: writeObject\n  6: readObject\n  7: writeObjectList\n  8: readObjectList\n  9: modifyObjectInList\n  q: quit\n");

		action = _getch();
		wprintf(L"\n");

		if(!pInventory)
		{
			if((action >= '5') && (action <= '9'))
				action = 0;
		}

		switch(action)
		{
		case '1':
			wprintf(L"1: writeAttributes\n");
			writeAttributes(pSupplies);
			break;

		case '2':
			wprintf(L"2: readAttributes\n");
			readAttributes(pSupplies);
			break;

		case '3':
			wprintf(L"3: writeList\n");
			writeList(pSupplies);
			break;

		case '4':
			wprintf(L"4: readList\n");
			readList(pSupplies);
			break;

		case '5':
			wprintf(L"5: writeObject\n");
			writeObject(pInventory);
			break;

		case '6':
			wprintf(L"6: readObject\n");
			readObject(pInventory);
			break;

		case '7':
			wprintf(L"7: writeObjectList\n");
			writeObjectList(pInventory);
			break;

		case '8':
			wprintf(L"8: readObjectList\n");
			readObjectList(pInventory);
			break;

		case '9':
			wprintf(L"a: modifyObjectInList\n");
			modifyObjectInList(pInventory);
			break;

		default:
			wprintf(L"invalid selection\n");
			break;

		case 'q':
			break;
		}

	} while(action != 'q');

	pSupplies->Release();
	if(pInventory)
		pInventory->Release();

	uninit(pConnection, pDomain, ulStorageId);

	return 0;
}
