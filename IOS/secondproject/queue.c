#include "StrunctDataAndFunctions.h"


void Next(SemaphoresStruct* sem)
{
	int boolFlags[7];
	boolFlags[0] = (sem->semap_H = init("semaphore_hydrogen", 0)) == SEM_FAILED;
	boolFlags[1] = (sem->semap_O = init("semaphore_oxygen", 1)) == SEM_FAILED;
	boolFlags[2] = (sem->semap_fin = init("semaphore_output", 1)) == SEM_FAILED;
	boolFlags[3] = (sem->sem_ready = init("semaphore_ready", 0)) == SEM_FAILED;
	boolFlags[4] = (sem->sem_created = init("semaphore_create", 0)) == SEM_FAILED;
	boolFlags[5] = (sem->sem_creating = init("semaphore_creating", 0)) == SEM_FAILED;
	boolFlags[6] = (sem->sem__stop = init("semaphore_stop_extra", 0)) == SEM_FAILED;

	for (int index = 0; index < 7; index++)
		if (boolFlags[index] == 1) {
			perror("sem_open");
			exit(1);
		}

	sem_unlink("semaphore_hydrogen");
	sem_unlink("semaphore_oxygen");
	sem_unlink("semaphore_output");
	sem_unlink("semaphore_ready");
	sem_unlink("semaphore_create");
	sem_unlink("semaphore_creating");
	sem_unlink("semaphore_stop_extra");
}


void Prev(SemaphoresStruct*semaphor) {
	sem_close(semaphor->semap_H);
	sem_close(semaphor->semap_O);
	sem_close(semaphor->semap_fin);
	sem_close(semaphor->sem_ready);
	sem_close(semaphor->sem_created);
	sem_close(semaphor->sem_creating);
	sem_close(semaphor->sem__stop);
}