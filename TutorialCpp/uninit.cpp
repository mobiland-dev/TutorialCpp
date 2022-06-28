#include "pch.h"

void uninit(WDomain*& pDomain, UINT32 ulStorageId)
{
	// unbind from schema
	AccessDefinition::Unbind();

	// disconnect from storage
	pDomain->ReleaseStorage(ulStorageId);

	// disconnect from server
	pDomain->DisconnectAll();

	// give more time for cleanup (prevents false error messages from the debugger)
	::Sleep(400);

	// delete domain object
	pDomain->Uninitialize();
	Domain_Destroy(pDomain);

	// uninitialize thread and free libraries
	UninitializeThread();
}
