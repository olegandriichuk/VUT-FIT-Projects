#include "StrunctDataAndFunctions.h"

ProcessDataStruct createProcess(unsigned int countOfAtoms, const char atomSymbol)
{
	for (id_t IDAtom = 1; IDAtom <= countOfAtoms; IDAtom++)
		if (fork() == 0)
			return PROCESS_CREATE(getpid(), 
				   ATOM_CREATE(IDAtom, atomSymbol));

	return (ProcessDataStruct) { 0 };
}