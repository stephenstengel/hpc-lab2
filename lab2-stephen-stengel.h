#ifndef STEPHEN_STENGEL_LAB2lol
#define STEPHEN_STENGEL_LAB2lol

#include <stdio.h>
#include <omp.h>

#include "mylab1functions.h"


#define MAX_FILENAME_LEN 500
#define MAX_STR_LEN 50




//function definitions
void testStringPass(char * dataStr);
void writeDataToFile(char * filename, int m, int n, int t, double time);
void doAlmostNothing();


//functions

//Appends a (m n t time) datapoint to the end of the given filename.
void writeDataToFile(char * filename, int m, int n, int t, double time)
{
	FILE *myFile = fopen(filename, "a");
	fprintf(myFile, "%d %d %d %f\n", m, n, t, time);
	fclose(myFile);
}


int runThreadTest(char * dataFile, char * picsFile, int maxM, int N, int maxT)
{
	//~ printf("enter any number to continue...\n");
	//~ int a;
	//~ scanf("%d", &a);
	
	//~ printf("num threads in pool: %d\n", omp_get_num_threads());
	
	//create timer variables.
	struct timespec start, finish;
	double lowest = INT_MAX;
	double elapsedTime = -1;
	
	
	for (int tee = 1; tee <= maxT; tee++)
	{
		#pragma omp parallel num_threads(tee)
		{
			#pragma omp single nowait
			printf("num threads in use: %d\n", omp_get_num_threads());
		
			#pragma omp for private(start, finish, lowest, elapsedTime)
			for (int i = 1; i < maxM + 1; i++)
			{
				//create array
				int sizeThisRound = i;
				//~ int teeThisRound = j;
				double **myArray = createSquareArray(sizeThisRound);
				myArray = fillSquareArrayRandomDoubles(myArray, sizeThisRound);
				
				//do three tests timing each
				lowest = INT_MAX;
				for (int k = 0; k < 3; k++)
				{
					clock_gettime(CLOCK_MONOTONIC, &start);
					powerArrays(myArray, sizeThisRound, N);
					elapsedTime = getElapsedTime(start, finish);
					if (elapsedTime < lowest)
					{
						lowest = elapsedTime;
					}
				}
		
				//~ //append the i, j, time to a file
				#pragma omp critical
				{
					writeDataToFile(dataFile, sizeThisRound, N, omp_get_num_threads(), lowest);
				}
				freeSquareDoubleArray(myArray, sizeThisRound);
			}
		}//end of parallel section.
	}//end of loop that varies num threads
	
	printf("runthreadtest complete!\n");
	return 0;
}

//Do almost nothing, used to make sure threads are created at start and not as needed.
void doAlmostNothing()
{
	int a = 0;
	int b = 0;
	a += b;
}

#endif
