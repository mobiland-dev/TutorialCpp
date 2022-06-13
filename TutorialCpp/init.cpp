#include "pch.h"

HRESULT init(WDomain*& pDomain, const wchar_t* strServerAddress, UINT16 usServerPort, const GUID& guidDomainId, UINT32 ulStorageId)
{
	HRESULT hRes;

	// load libraries and initialize thread
	InitializeThread();

	// build domain object
	pDomain = Domain_Create();

	if(FAILED(hRes = pDomain->Initialize(&guidDomainId)))
	{
		printf("Initialize failed (0x%x)\n", hRes);
		Domain_Destroy(pDomain);
		UninitializeThread();
		return E_FAIL;
	}

	// connect to server
	if(FAILED(hRes = pDomain->Connect(strServerAddress, usServerPort, NULL)))
	{
		printf("Connect failed (0x%x)\n", hRes);
		pDomain->Uninitialize();
		Domain_Destroy(pDomain);
		UninitializeThread();
		return E_FAIL;
	}

	// connect to storage
	if(FAILED(hRes = pDomain->QueryStorage(ulStorageId, false)))
	{
		printf("QueryStorage failed (0x%x)\n", hRes);
		pDomain->DisconnectAll();
		pDomain->Uninitialize();
		Domain_Destroy(pDomain);
		UninitializeThread();
		return E_FAIL;
	}

	// bind to schema
	if(FAILED(hRes = AccessDefinition::Bind(pDomain)))
	{
		printf("Bind failed (0x%x)\n", hRes);
		pDomain->ReleaseStorage(ulStorageId);
		pDomain->DisconnectAll();
		pDomain->Uninitialize();
		Domain_Destroy(pDomain);
		UninitializeThread();
		return E_FAIL;
	}

	return S_OK;
}
