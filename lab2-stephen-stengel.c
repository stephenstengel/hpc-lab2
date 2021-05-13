//Stephen Stengel  <stephen.stengel@cwu.edu>  40819903
//Lab2  parallel matrix multiplications


//Using MAX_FILENAME_LEN for a few things.

//my stuff
#include "lab2-stephen-stengel.h"
#include "mylab1functions.h"

int main(int argc, char **argv)
{
	printf("Hi!\n");
	printf("I already know that N will just be a linear increase, "\
			"so this test will just use one value of N for all sub-tests.\n");
	
	checkForHelpCommands(argc, argv);
	checkInput(argc, argv);
	int maxM = atoi(argv[1]);
	int N = atoi(argv[2]);
	int maxT = atoi(argv[3]);
	
	//Set up thread pool. Threads are not killed till end of program.
	omp_set_num_threads(maxT); //sets the DEFAULT number of threads to create/use in a parallel section.
	
	//~ printf("Max vals...\nM: %d\tN: %d\tT: %d\n", maxM, N, maxT);
	
	//create a filename structure for easy scripting.
	char dataDirectory[] = "../data/";
	char picsDirectory[] = "../pics/";
	//~ testStringPass(dataDirectory);
	
	
	
	//CELL//////
	char thisDataName[MAX_FILENAME_LEN];
	sprintf(thisDataName, "%scell-m%05dn%05dt%05d", dataDirectory, maxM, N, maxT);
	char thisPicsName[MAX_FILENAME_LEN];
	sprintf(thisPicsName, "%scell-m%05dn%05dt%05d", picsDirectory, maxM, N, maxT);
	printf("Data file path and name:\t%s\n", thisDataName);
	printf("Pics file path and name:\t%s\n", thisPicsName);
	
	deleteFileIfExists(thisDataName); //So that only data from the current program is saved.
	//maybe make a file to keep track of the current version. Then I can use multiple runs of data safely.
	
	char cellString[] = "Cell";
	runFuncTest(multSquareArraysThreadCell, thisDataName, thisPicsName, maxM, N, maxT, cellString);
	
	
	//Might be able to use same filenames and print on same graph with labels? do this for now.
	//ROW//////
	//~ thisDataName[MAX_FILENAME_LEN];
	sprintf(thisDataName, "%srow-m%05dn%05dt%05d", dataDirectory, maxM, N, maxT);
	//~ thisPicsName[MAX_FILENAME_LEN];
	sprintf(thisPicsName, "%srow-m%05dn%05dt%05d", picsDirectory, maxM, N, maxT);
	printf("Data file path and name:\t%s\n", thisDataName);
	printf("Pics file path and name:\t%s\n", thisPicsName);
	
	deleteFileIfExists(thisDataName); //So that only data from the current program is saved.
	//maybe make a file to keep track of the current version. Then I can use multiple runs of data safely.
	
	char rowString[] = "Row";
	runFuncTest(multSquareArraysThreadRow, thisDataName, thisPicsName, maxM, N, maxT, rowString);
	
	
	//COL//////
	//~ thisDataName[MAX_FILENAME_LEN];
	sprintf(thisDataName, "%scol-m%05dn%05dt%05d", dataDirectory, maxM, N, maxT);
	//~ thisPicsName[MAX_FILENAME_LEN];
	sprintf(thisPicsName, "%scol-m%05dn%05dt%05d", picsDirectory, maxM, N, maxT);
	printf("Data file path and name:\t%s\n", thisDataName);
	printf("Pics file path and name:\t%s\n", thisPicsName);
	
	deleteFileIfExists(thisDataName); //So that only data from the current program is saved.
	
	char colString[] = "Col";
	runFuncTest(multSquareArraysThreadCol, thisDataName, thisPicsName, maxM, N, maxT, colString);
	

	
	return 0;
}


//~ void testStringPass(char dataStr[])//this is also valid as is naming the size
void testStringPass(char * dataStr)
{
	printf("The passed string: %s\n", dataStr);
}
