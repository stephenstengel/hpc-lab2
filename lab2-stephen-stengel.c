//Stephen Stengel  <stephen.stengel@cwu.edu>  40819903
//Lab2  parallel matrix multiplications


//Using MAX_FILENAME_LEN for a few things.

//my stuff
#include "lab2-stephen-stengel.h"
#include "mylab1functions.h"

int main(int argc, char **argv)
{
	printf("Hi!\n");
	
	checkForHelpCommands(argc, argv);
	checkInput(argc, argv);
	int maxM = atoi(argv[1]);
	int maxN = atoi(argv[2]);
	int maxT = atoi(argv[3]);
	
	printf("Max vals...\nM: %d\tN: %d\tT: %d\n", maxM, maxN, maxT);
	
	//need to create a filename structure for easy scripting.
	char dataDirectory[MAX_STR_LEN] = "../data/";
	char picsDirectory[MAX_STR_LEN] = "../pics/";
	printf("data directory: %s\npics directory: %s\n", dataDirectory, picsDirectory);
	
	testStringPass(dataDirectory);
	testStringPass(picsDirectory);
	
	char stringM[MAX_STR_LEN];
	sprintf(stringM, "%d", maxM);
	
	char stringN[MAX_STR_LEN];
	sprintf(stringN, "%d", maxN);
	
	char stringT[MAX_STR_LEN];
	sprintf(stringT, "%d", maxT);
	
	char thisDataName[MAX_FILENAME_LEN];
	sprintf(thisDataName, "%sm%05dn%05dt%05d", dataDirectory, maxM, maxN, maxT);
	char thisPicsName[MAX_FILENAME_LEN];
	sprintf(thisPicsName, "%sm%05dn%05dt%05d", picsDirectory, maxM, maxN, maxT);
	
	printf("Data file path and name on next line:\n%s\n", thisDataName);
	printf("Pics file path and name on next line:\n%s\n", thisPicsName);
	
	deleteFileIfExists(thisDataName);
	deleteFileIfExists(thisPicsName);
	
	
	return 0;
}


//~ void testStringPass(char dataStr[])//this is also valid as is naming the size
void testStringPass(char * dataStr)
{
	printf("The passed string: %s\n", dataStr);
}
