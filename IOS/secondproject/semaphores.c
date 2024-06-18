#include "StrunctDataAndFunctions.h"

void PrintStatusOfAtomAndSemaphore(const int IdStatus, ProcessDataStruct *ProcessStruct, InterProcessCommunicationsData *InnerPCData)
{
	FILE *Out = fopen("./proj2.out", "a");
	++InnerPCData->num;

	if (IdStatus == 20)
		fprintf(Out,"%u: %c %u: started\n", InnerPCData->num, ProcessStruct->Elem.symbol, ProcessStruct->Elem.IDAtom);
	if (IdStatus == 21)
		fprintf(Out, "%u: %c %u: going to queue\n", InnerPCData->num, ProcessStruct->Elem.symbol, ProcessStruct->Elem.IDAtom);
	if (IdStatus == 22)
		fprintf(Out, "%u: %c %u: creating molecule %u\n", InnerPCData->num, ProcessStruct->Elem.symbol, ProcessStruct->Elem.IDAtom, InnerPCData->IDMolecule);
	if (IdStatus == 23)
		fprintf(Out, "%u: %c %u: molecule %u created\n", InnerPCData->num, ProcessStruct->Elem.symbol, ProcessStruct->Elem.IDAtom, InnerPCData->IDMolecule);
	if (IdStatus == 24)
		fprintf(Out, "%u: %c %u: not enough O or H\n", InnerPCData->num, ProcessStruct->Elem.symbol, ProcessStruct->Elem.IDAtom);
	if (IdStatus == 25)
		fprintf(Out, "%u: %c %u: not enough H\n", InnerPCData->num, ProcessStruct->Elem.symbol, ProcessStruct->Elem.IDAtom);

	fclose(Out);
}


int ArgumentsFunc(DataArguments *StructOfArguments, int args_count, char **ArrayOfArgumens)
{
	if (args_count != 5)
		return 10;

	for (int number = 1; number < args_count; number++)
	{
		if (ArrayOfArgumens[number][0] == '\0')
			return 10;

		for (int argNum = 0; ArrayOfArgumens[number][argNum]; argNum++)
			if (!(((ArrayOfArgumens[number][argNum]) >= '0') && (ArrayOfArgumens[number][argNum]) <= '9'))
				return 10;

		unsigned int ArgNum = (unsigned)atoi(ArrayOfArgumens[number]);

		if (number == 1)
		{
			if (ArgNum < 1) return 1;
			StructOfArguments->NO = ArgNum;
		}
		else if (number == 2)
		{
			if (ArgNum < 1)	return 1;
			StructOfArguments->NH = ArgNum;
		}
		else if (number == 3)
		{
			if (ArgNum > 1000) return 1;
			StructOfArguments->TI = ArgNum;
		}
		else
		{
			if (ArgNum > 1000) return 1;
			StructOfArguments->TB = ArgNum;
			break;
		}
	}
	StructOfArguments->Count_H_Atom = StructOfArguments->NH;
	StructOfArguments->Count_O_Atom = StructOfArguments->NO;
	if (StructOfArguments->NH % 2 == 1)
		StructOfArguments->Count_H_Atom--;
	if (StructOfArguments->NH >= StructOfArguments->NO * 2)
		StructOfArguments->Count_H_Atom = StructOfArguments->Count_O_Atom * 2;
	else
		StructOfArguments->Count_O_Atom = StructOfArguments->Count_H_Atom / 2;

	return 0;
}

