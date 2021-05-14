#ifndef STEPHEN_STENGEL_LAB2lol
#define STEPHEN_STENGEL_LAB2lol

#include <stdio.h>
#include <omp.h>

#include "mylab1functions.h"


#define MAX_FILENAME_LEN 500
#define MAX_STR_LEN 50




//function definitions
void testStringPass(char * dataStr);
void writeDataToFile(char * filename, int m, int n, int t, double time, char * testType);
void doAlmostNothing();
int runFuncTest(double **myFunc(), char * dataFile, char * picsFile, int maxM, int N, int maxT, char * testType);
void timeThisWay(double **myFunc(), double **myArray, char *dataFile, char *picsFile, int m, int N, int tee, char *testType);
double powerArraysThisWay(double **myFunc(), double **myArray, int m, int N, int tee);

//functions

//Appends a (m n t time) datapoint to the end of the given filename.
void writeDataToFile(char * filename, int m, int n, int t, double time, char * testType)
{
	FILE *myFile = fopen(filename, "a");
	fprintf(myFile, "%d %d %d %f %s\n", m, n, t, time, testType);
	fclose(myFile);
}




//This tests the time of the given function. col, row, or individual. maybe block if I have time.
//The given function will handle printing? This function will handle the parameters going to the
//test function.
int runFuncTest(double **myFunc(), char * dataFile, char * picsFile, int maxM, int N, int maxT, char * testType)
{
	int numThreadTests = 10;
	int tIncrement = 1;
	if ((maxT / numThreadTests) > 1)
	{
		tIncrement = (maxT / numThreadTests);
	}
	printf("tIncrement: %d\n", tIncrement);
	
	//~ for (int tee = 1; tee <= maxT; tee += tIncrement)
	//~ for (int tee = maxT; tee > 0; tee -= tIncrement)
	
	int mStep = 1;
	if ( maxM > 100 )
	{
		mStep = maxM / 20; //20 chosen because it is a good balance.
	}
	
	int tTop = maxT - (maxT - (tIncrement * (numThreadTests - 1))) + 1;
	printf("tTop: %d\n", tTop);
	for (int tee = tTop; tee > 0; tee -= tIncrement)//backwards keeps openMP from killing useful threads
	{
		printf("This T: %d\n", tee);
		for (int m = 1; m < maxM + 1; m += mStep)
		{
			double **myArray = createSquareArray(m);
			myArray = fillSquareArrayRandomDoubles(myArray, m);
			
			//This function will find the min of three times for these inputs and write data to file.
			timeThisWay(myFunc, myArray, dataFile, picsFile, m, N, tee, testType);
			freeSquareDoubleArray(myArray, m);
		}
	}//end of loop that varies num threads
	
	printf("runfunctest: %s complete! now printing to file...\n", testType);
	//make graph
	char printCommandCell[MAX_COMMAND_LEN];
	sprintf(printCommandCell, "python3 cArrayGraph.py %s %s %s", dataFile, picsFile, testType);
	
	system( printCommandCell );
	printf("done!");
	
	return 0;
}

//gets the time of doin it this way
void timeThisWay(double **myFunc(), double **myArray, char *dataFile, char *picsFile, int m, int N, int tee, char *testType)
{
	//~ #pragma omp parallel 
	//~ {
		//~ //Prevent threads from being deleted due to timeout.
	//~ }

	double lowest = INT_MAX;
	double elapsedTime = -1;
	
	//do three tests timing each
	for (int k = 0; k < 3; k++)
	{
		elapsedTime = powerArraysThisWay(myFunc, myArray, m, N, tee);
		if (elapsedTime < lowest)
		{
			lowest = elapsedTime;
		}
	}
	
	//~ //append the i, j, time to a file
	writeDataToFile(dataFile, m, N, tee, lowest, testType);
}

//calls the specific multiplication func needed and returns runtime minus thread creation (should be zero because openmp keeps threads sleeping)
double powerArraysThisWay(double **myFunc(), double **myArray, int m, int N, int tee)
{
	//~ #pragma omp parallel
	//~ {
		//~ //Prevent threads from being deleted due to timeout.
	//~ }
	
	double **originalCopy = copySquareDoubleArray(myArray, m);
	double **intermediate = copySquareDoubleArray(myArray, m);
	
	//create timing vars here
	struct timespec start, finish;
	double elapsedTime = -1;
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (int i = 1; i < N; i++)
	{
		double **output = createSquareArray(m);
		output = myFunc(originalCopy, intermediate, output, m, tee);

		//copy output into intermediate
		intermediate = copySquareDoubleArray(output, m);
		
		freeSquareDoubleArray(output, m);
	}
	elapsedTime = getElapsedTime(start, finish);	
					
	freeSquareDoubleArray(intermediate, m);
	freeSquareDoubleArray(originalCopy, m);
	
	return elapsedTime;
}

double **multSquareArraysThreadCell(double **original, double **intermediate, double **output, int size, int tee)
{
	int i = 0;
	int j = 0;
	int k = 0;
	#pragma omp parallel num_threads(tee) //only use this many threads out of the threadpool
	{
		//~ printf("current T: %d\tnumThreads: %d\n", tee, omp_get_num_threads());
		#pragma omp for collapse(2) private(i, j, k) nowait //each thread loops through the k's for each cell
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				double tmp = output[i][j];
				for (k = 0; k < size; k++)
				{
					tmp += original[i][k] * intermediate[k][j];
				}
				output[i][j] = tmp;
			}
		}
	}
	
	return output;
}

double **multSquareArraysThreadRow(double **original, double **intermediate, double **output, int size, int tee)
{
	int i = 0;
	int j = 0;
	int k = 0;
	#pragma omp parallel num_threads(tee) //only use this many threads out of the threadpool
	{
		#pragma omp for private(i, j, k) nowait //each thread has its own row i.
		for (i = 0; i < size; i++)
		{
			for (j = 0; j < size; j++)
			{
				double tmp = output[i][j];
				for (k = 0; k < size; k++)
				{
					//critical here? no, each thread has own spot.
					//false sharing though, need to move array copies into this parallel section.
					tmp += original[i][k] * intermediate[k][j];
				}
				output[i][j] = tmp;
			}
		}
	}
	
	return output;
}


//Same as row but with the i and j loops switched.
double **multSquareArraysThreadCol(double **original, double **intermediate, double **output, int size, int tee)
{
	int i = 0;
	int j = 0;
	int k = 0;
	#pragma omp parallel num_threads(tee) //only use this many threads out of the threadpool
	{
		#pragma omp for private(i, j, k) nowait //each thread has its own col j.
		for (j = 0; j < size; j++)
		{
			for (i = 0; i < size; i++)
			{
				double tmp = output[i][j];
				for (k = 0; k < size; k++)
				{
					tmp += original[i][k] * intermediate[k][j];
				}
				output[i][j] = tmp;
			}
		}
	}
	
	return output;
}

//Do almost nothing, used to make sure threads are created at start and not as needed.
void doAlmostNothing()
{
	int a = 0;
	int b = 0;
	a += b;
}

#endif



//Unused
/*
//this tests times for running each separate thread size
int runThreadTest(char * dataFile, char * picsFile, int maxM, int N, int maxT, char * testType)
{
	//~ printf("enter any number to continue...\n");
	//~ int a;
	//~ scanf("%d", &a);
	
	//~ printf("num threads in pool: %d\n", omp_get_num_threads());
	
	//create timer variables.
	struct timespec start, finish;
	double lowest = INT_MAX;
	double elapsedTime = -1;
	
	int tIncrement = 1;
	if ((maxT / 10) > 1)
	{
		tIncrement = maxT / 10;
	}
	printf("tIncrement: %d\n", tIncrement);
	for (int tee = 1; tee <= maxT; tee += tIncrement)
	{
		//~ #pragma omp parallel num_threads(tee)
		{
			//~ #pragma omp single nowait
			printf("num threads in use: %d\n", omp_get_num_threads());
		
			//~ #pragma omp for private(start, finish, lowest, elapsedTime) nowait
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
				//~ #pragma omp critical
				{
					//~ writeDataToFile(dataFile, sizeThisRound, N, omp_get_num_threads(), lowest, testType);
					writeDataToFile(dataFile, sizeThisRound, N, tee, lowest, testType);
				}
				freeSquareDoubleArray(myArray, sizeThisRound);
			}
		}//end of parallel section.
	}//end of loop that varies num threads
	
	printf("runthreadtest complete!\n");
	return 0;
}
*/
