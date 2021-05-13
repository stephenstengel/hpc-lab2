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
	#pragma omp parallel num_threads(maxT) //to be explicit as well
	{
		#pragma omp single
		printf("startup:\tnum threads: %d\n", omp_get_num_threads());
		
		#pragma omp for
		for (int i = 0; i < maxT; i++)
		{
			doAlmostNothing();
			printf("id: %d\tnum threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
			
		}
		
	}
	
	
	printf("Max vals...\nM: %d\tN: %d\tT: %d\n", maxM, N, maxT);
	
	//create a filename structure for easy scripting.
	char dataDirectory[] = "../data/";
	char picsDirectory[] = "../pics/";
	//~ testStringPass(dataDirectory);
	
	char thisDataName[MAX_FILENAME_LEN];
	sprintf(thisDataName, "%sm%05dn%05dt%05d", dataDirectory, maxM, N, maxT);
	char thisPicsName[MAX_FILENAME_LEN];
	sprintf(thisPicsName, "%sm%05dn%05dt%05d", picsDirectory, maxM, N, maxT);
	
	printf("Data file path and name on next line:\n%s\n", thisDataName);
	printf("Pics file path and name on next line:\n%s\n", thisPicsName);
	
	deleteFileIfExists(thisDataName);
	deleteFileIfExists(thisPicsName);
	
	//~ printf("enter any number to continue...\n");
	//~ int a;
	//~ scanf("%d", &a);
	
	
	//~ char testType[] = "overall";
	//~ runThreadTest(thisDataName, thisPicsName, maxM, N, maxT, testType);
	
	char cellString[] = "Cell";
	runFuncTest(multSquareArraysThreadCell, thisDataName, thisPicsName, maxM, N, maxT, cellString);
	

	
	return 0;
}


//~ void testStringPass(char dataStr[])//this is also valid as is naming the size
void testStringPass(char * dataStr)
{
	printf("The passed string: %s\n", dataStr);
}
