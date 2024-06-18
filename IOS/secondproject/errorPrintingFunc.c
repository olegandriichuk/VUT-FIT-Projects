#include "StrunctDataAndFunctions.h"

void ErrorsFunc(int IDcodeOfError) {
	if (IDcodeOfError == 0)
		return;

	if (IDcodeOfError == 0)
		fprintf(stderr, "ERROR: Invalid input arguments\n");
	else
		fprintf(stderr, "ERROR: Unknown error IDAtom\n");

	exit(1);
}