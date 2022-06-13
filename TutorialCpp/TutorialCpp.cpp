#include "pch.h"

const GUID guidEntryPoint
{ 0x71e456a5, 0x6a4e, 0x46aa, { 0xa7, 0x27, 0x10, 0x5d, 0x58, 0x4c, 0x79, 0x17 } };

HRESULT	init				(WDomain*& pDomain, const wchar_t* strServerAddress, UINT16 usServerPort, const GUID& guidDomainId, UINT32 ulStorageId);
void	uninit				(WDomain*& pDomain, UINT32 ulStorageId);

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

	WDomain* pDomain;

	if(S_OK != init(pDomain, strServerAddress, usServerPort, guidDomainId, ulStorageId))
	{
		printf("init failed\n");
		return -1;
	}

	int action;
	WSupplies* pSupplies = NULL;
	WInventory* pInventory = NULL;

	do
	{
		wprintf(L"\n 1: create new Supplies object\n 2: open existing Supplies object\n\n 3: create new Inventory object\n 4: extend Supplies object to Inventory object\n 5: open existing Inventory object\n\nselect action: ");
		action = _getch();

		switch(action)
		{
		case '1':
			{
				WSupplies::Create(&pSupplies, pDomain);

				pDomain->InsertNamedObject(&pSupplies->BuildLink(true), &guidEntryPoint, L"basic entry point");
			}
			break;

		case '2':
			{
				DataFoundation::stcObjectLink* pSuppliesLink;
				if(S_OK == pDomain->QueryNamedObjectLink(&guidEntryPoint, 1, &pSuppliesLink))
				{
					if(WSupplies::IsOfType(pSuppliesLink))
					{
						WSupplies::Open(&pSupplies, *pSuppliesLink, pDomain);
						pSupplies->Load();

						DataFoundationAccess::MemFree(pSuppliesLink);

						if(FAILED(pDomain->Execute(Transaction::Load)))
						{
							wprintf(L"Execute failed...\n");
							uninit(pDomain, ulStorageId);
							return -1;
						}
					}
					else
					{
						DataFoundationAccess::MemFree(pSuppliesLink);

						wprintf(L"Object is not of type WSupplies\n");
					}
				}
				else
					wprintf(L"Entry point not found\n");
			}
			break;

		case '3':
			{
				WInventory::Create(&pInventory, pDomain);

				pDomain->InsertNamedObject(&pInventory->BuildLink(true), &guidEntryPoint, L"extended entry point");

				pSupplies = WSupplies::CastTo(pInventory);
			}
			break;

		case '4':
			{
				DataFoundation::stcObjectLink* pSuppliesLink;
				if(S_OK == pDomain->QueryNamedObjectLink(&guidEntryPoint, 1, &pSuppliesLink))
				{
					if(WInventory::IsOfType(pSuppliesLink))
					{
						DataFoundationAccess::MemFree(pSuppliesLink);

						wprintf(L"Object is already of type WInventory\n");
					}
					else if(WSupplies::IsOfType(pSuppliesLink))
					{
						WSupplies* pSupplies = NULL;
						WSupplies::Open(&pSupplies, *pSuppliesLink, pDomain);

						if(FAILED(pDomain->Execute(Transaction::Load)))
						{
							wprintf(L"Execute failed...\n");
							uninit(pDomain, ulStorageId);
							return -1;
						}

						WInventory* pInventory;
						WInventory::Extend(&pInventory, pSupplies);

						pDomain->InsertNamedObject(&pInventory->BuildLink(true), &guidEntryPoint, L"updated entry point");
					}
					else
					{
						DataFoundationAccess::MemFree(pSuppliesLink);

						wprintf(L"Object is not of type WSupplies\n");
					}
				}
				else
					wprintf(L"Entry point not found\n");
			}
			break;

		case '5':
			{
				DataFoundation::stcObjectLink* pInventoryLink;
				if(S_OK == pDomain->QueryNamedObjectLink(&guidEntryPoint, 1, &pInventoryLink))
				{
					if(WInventory::IsOfType(pInventoryLink))
					{
						WInventory::Open(&pInventory, *pInventoryLink, pDomain);
						pInventory->Load();

						DataFoundationAccess::MemFree(pInventoryLink);

						if(FAILED(pDomain->Execute(Transaction::Load)))
						{
							wprintf(L"Execute failed...\n");
							uninit(pDomain, ulStorageId);
							return -1;
						}

						pSupplies = WSupplies::CastTo(pInventory);
					}
					else if(WSupplies::IsOfType(pInventoryLink))
					{
						DataFoundationAccess::MemFree(pInventoryLink);

						wprintf(L"Object is not of type WInventory but of type WSupplies\n");
					}
					else
					{
						DataFoundationAccess::MemFree(pInventoryLink);

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
		wprintf(L"select function:\n  1: writeAttributes\n  2: readAttribute\n  3: writeList\n  4: readList\n  5: writeObject\n  6: readObject\n  7: writeObjectList\n  8: readObjectList\n  9: modifyObjectInList\n  q: quit\n");
		action = _getch();

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

	uninit(pDomain, ulStorageId);

	return 0;
}
