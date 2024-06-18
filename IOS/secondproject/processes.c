#include "StrunctDataAndFunctions.h"

InterProcessCommunicationsData* InterProcessCommunicationsStart()
{
	key_t ProcessKey = ftok("mainFunc.c", 'c');
	int InterProcessCommunicationsKey = shmget(ProcessKey, sizeof(InterProcessCommunicationsData), 0666 | IPC_CREAT);
	InterProcessCommunicationsData* innerPC = shmat(InterProcessCommunicationsKey, NULL, 0);
	innerPC->ID_IPC_key = InterProcessCommunicationsKey;

	return innerPC;
}