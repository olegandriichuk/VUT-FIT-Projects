#include "StrunctDataAndFunctions.h"

void MemoryFree(InterProcessCommunicationsData*interProcessCommunications) 
{
	key_t childProcessKey = interProcessCommunications->ID_IPC_key;
	shmctl(childProcessKey, IPC_RMID, NULL);
	shmdt(interProcessCommunications);
}