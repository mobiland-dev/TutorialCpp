#include "pch.h"

void uninit(WDomain*& pDomain, UINT32 ulStorageId)
{
	// unbind from schema
	AccessDefinition::Unbind();

	// disconnect from storage
	pDomain->ReleaseStorage(ulStorageId);

	// disconnect from server
	pDomain->DisconnectAll();

	// delete domain object
	pDomain->Uninitialize();
	Domain_Destroy(pDomain);

	// uninitialize thread and free libraries
	UninitializeThread();
}
