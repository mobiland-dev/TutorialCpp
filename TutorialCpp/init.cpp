#include "pch.h"

HRESULT init(WDomain*& pDomain, const wchar_t* strServerAddress, UINT16 usServerPort, const GUID& guidDomainId, UINT32 ulStorageId)
{
	HRESULT hRes;

	// load libraries and initialize thread
	InitializeThread();

	// build connection object
	Connection* pConnection = Connection_Create();

	if(FAILED(hRes = pConnection->Initialize(&guidDomainId)))
	{
		printf("Initialize connection failed (0x%x)\n", hRes);
		Connection_Destroy(pConnection);
		UninitializeThread();
		return E_FAIL;
	}

	// connect to server
	if(FAILED(hRes = pConnection->Connect(strServerAddress, usServerPort, NULL)))
	{
		printf("Connect failed (0x%x)\n", hRes);
		pConnection->Uninitialize();
		Connection_Destroy(pConnection);
		UninitializeThread();
		return E_FAIL;
	}

	// connect to storage
	if(FAILED(hRes = pConnection->QueryStorage(ulStorageId, false)))
	{
		printf("QueryStorage failed (0x%x)\n", hRes);
		pConnection->DisconnectAll();
		pConnection->Uninitialize();
		Connection_Destroy(pConnection);
		UninitializeThread();
		return E_FAIL;
	}

	// build domain object
	pDomain = Domain_Create();

	if(FAILED(hRes = pDomain->Initialize(pConnection, 0)))
	{
		printf("Initialize failed (0x%x)\n", hRes);
		Domain_Destroy(pDomain);
		pConnection->ReleaseAllStorages();
		pConnection->DisconnectAll();
		pConnection->Uninitialize();
		Connection_Destroy(pConnection);
		UninitializeThread();
		return E_FAIL;
	}

	// bind to schema
	if(FAILED(hRes = AccessDefinition::Bind(pDomain)))
	{
		printf("Bind failed (0x%x)\n", hRes);
		pDomain->Uninitialize();
		Domain_Destroy(pDomain);
		pConnection->ReleaseAllStorages();
		pConnection->DisconnectAll();
		pConnection->Uninitialize();
		Connection_Destroy(pConnection);
		UninitializeThread();
		return E_FAIL;
	}

	return S_OK;
}
