#include "pch.h"

void uninit(WDomain*& pDomain, UINT32 ulStorageId)
{
	Connection* pConnection = pDomain->GetConnection();

	// unbind from schema
	AccessDefinition::Unbind();

	// delete domain object
	pDomain->Uninitialize();
	Domain_Destroy(pDomain);

	// disconnect from storage
	pConnection->ReleaseAllStorages();

	// disconnect from server
	pConnection->DisconnectAll();

	// delete connection object
	pConnection->Uninitialize();
	Connection_Destroy(pConnection);

	// uninitialize thread and free libraries
	UninitializeThread();
}
