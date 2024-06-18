#include "StrunctDataAndFunctions.h"

int main(int countOfArgiments, char **argumentsArrayVar)
{
	remove("./proj2.out");
	SemaphoresStruct testsemaphore;
	ProcessDataStruct proc;
	InterProcessCommunicationsData *ipcproc = NULL;
	DataArguments ArrayOfArguments;

	pid_t IDMainProcess = getpid();
	ErrorsFunc(ArgumentsFunc(&ArrayOfArguments, countOfArgiments, argumentsArrayVar));
	ipcproc = InterProcessCommunicationsStart();
	Next(&testsemaphore);

	if (ArrayOfArguments.Count_H_Atom == 0 && ArrayOfArguments.Count_O_Atom == 0)
		sem_post(testsemaphore.sem__stop);
	if (IDMainProcess == getpid())
		proc = createProcess(ArrayOfArguments.NO, 'O');
	if (IDMainProcess == getpid())
		proc = createProcess(ArrayOfArguments.NH, 'H');

	if (IDMainProcess != getpid())
	{
		srand(getpid());
		sem_wait(testsemaphore.semap_fin);
		PrintStatusOfAtomAndSemaphore(20, &proc, ipcproc);
		sem_post(testsemaphore.semap_fin);
		usleep(ArrayOfArguments.TI > 0 ? rand() % (ArrayOfArguments.TI) * 1000 : 0);
		sem_wait(testsemaphore.semap_fin);
		PrintStatusOfAtomAndSemaphore(21, &proc, ipcproc);
		sem_post(testsemaphore.semap_fin);
		switch (proc.Elem.symbol)
		{
		case 'H':
			ipcproc->hydrogen_n++;
			proc.extra = ArrayOfArguments.Count_H_Atom < ipcproc->hydrogen_n;
			break;
		case 'O':
			ipcproc->oxygen_n++;
			proc.extra = ArrayOfArguments.Count_O_Atom < ipcproc->oxygen_n;
			break;
		}

		if (proc.extra == 1)
		{
			sem_wait(testsemaphore.sem__stop);
			PrintStatusOfAtomAndSemaphore(proc.Elem.symbol == 'H' ? 24 : 25, &proc, ipcproc);
			sem_post(testsemaphore.sem__stop);
			exit(0);
		}

		if (proc.Elem.symbol == 'H')
		{
			sem_wait(testsemaphore.semap_H);
			sem_wait(testsemaphore.semap_fin);
			PrintStatusOfAtomAndSemaphore(22, &proc, ipcproc);
			sem_post(testsemaphore.semap_fin);
			if (++ipcproc->NumOfMolecule_H >= 2)
			{
				sem_post(testsemaphore.sem_creating);
				sem_post(testsemaphore.sem_creating);
				sem_post(testsemaphore.sem_creating);
			}
			sem_wait(testsemaphore.sem_creating);
			sem_wait(testsemaphore.semap_fin);
			PrintStatusOfAtomAndSemaphore(23, &proc, ipcproc);
			sem_post(testsemaphore.semap_fin);
			--ipcproc->NumOfMolecule_H;
			sem_post(testsemaphore.sem_ready);

			exit(0);
		}
		else if (proc.Elem.symbol == 'O')
		{
			sem_wait(testsemaphore.semap_O);
			++ipcproc->IDMolecule;
			sem_wait(testsemaphore.semap_fin);
			PrintStatusOfAtomAndSemaphore(22, &proc, ipcproc);
			sem_post(testsemaphore.semap_fin);
			sem_post(testsemaphore.semap_H);
			sem_post(testsemaphore.semap_H);
			usleep(ArrayOfArguments.TB > 0 ? rand() % (ArrayOfArguments.TB) * 1000 : 0);
			sem_wait(testsemaphore.sem_creating);
			sem_wait(testsemaphore.semap_fin);
			PrintStatusOfAtomAndSemaphore(23, &proc, ipcproc);
			sem_post(testsemaphore.semap_fin);
			sem_wait(testsemaphore.sem_ready);
			sem_wait(testsemaphore.sem_ready);

			if (ArrayOfArguments.Count_O_Atom == ipcproc->IDMolecule)
				sem_post(testsemaphore.sem__stop);
			sem_post(testsemaphore.semap_O);
			exit(0);
		}
	}
	while (wait(NULL) > 0);
	Prev(&testsemaphore);
	MemoryFree(ipcproc);

	return 0;
}