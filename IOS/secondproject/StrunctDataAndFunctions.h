#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <semaphore.h>


typedef struct
{
	id_t IDAtom;
	char symbol;
} AtomData;


typedef struct
{
	pid_t IDAtom;
	AtomData Elem;
	unsigned int extra;
} ProcessDataStruct;


typedef struct
{
	unsigned int Count_H_Atom;
	unsigned int Count_O_Atom;
	unsigned int NH, NO, TI, TB;
} DataArguments;


typedef struct
{
	id_t IDMolecule;
	key_t ID_IPC_key;
	unsigned int NumOfMolecule_H;
	unsigned int num;
	unsigned int hydrogen_n;
	unsigned int oxygen_n;
} InterProcessCommunicationsData;


typedef struct
{
	sem_t *sem_ready;
	sem_t*sem_created;
	sem_t*sem_creating;
	sem_t*sem__stop;
	sem_t *semap_H;
	sem_t *semap_O;
	sem_t*semap_fin;
} SemaphoresStruct;



void ErrorsFunc(int);
ProcessDataStruct createProcess(unsigned int, const char);
int ArgumentsFunc(DataArguments *, int, char **);
ProcessDataStruct createProcess(unsigned int, const char);
void PrintStatusOfAtomAndSemaphore(const int, ProcessDataStruct *, InterProcessCommunicationsData *);
void Next(SemaphoresStruct*);
void Prev(SemaphoresStruct*);
InterProcessCommunicationsData* InterProcessCommunicationsStart();
void MemoryFree(InterProcessCommunicationsData*);
int ArgumentsFunc(DataArguments *, int, char **);
#define init(name,value)(sem_open(name, O_CREAT | O_WRONLY, 0666, value))
#define ATOM_CREATE(IDAtom, symbol) ((AtomData){IDAtom, symbol})
#define PROCESS_CREATE(IDAtom, Elem) ((ProcessDataStruct){IDAtom, Elem, 0})
