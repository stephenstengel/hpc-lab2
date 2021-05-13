//Stephen Stengel
//hpc lab1 header file

#ifndef STEPHEN_STENGEL_LAB1
#define STEPHEN_STENGEL_LAB1

#include <stdio.h>
#include <stdlib.h> 	//exit(), malloc()
#include <string.h> 	//strcmp()
#include <time.h> 		//time()
#include <limits.h> 	//INT_MAX
#include <math.h> 		//math functions
#include <ctype.h> 		//isspace(), isdigit(), etc.
//~ #include <stdbool.h> //bool type
#include <unistd.h>		//access() check file exists.

#define TRUE 1
#define FALSE 0

//These are a bit higher than what runs in a reasonable time. 100/10 is about an hour.
#define GLOBAL_MAX_M 125
#define GLOBAL_MAX_N 12
#define RAND_MULT 1000
#define MAX_COMMAND_LEN 2000

#define HELP_TEXT_PATH "helpfile"


struct node
{
	double value;
	int row;
	int col;
	struct node *right;
	struct node *down;
};


//Helper functions
int checkForHelpCommands(int argc, char** argv);
_Bool strIsHelp(char* aString);
void printTextFile(char* inputPath);
int* generateArray(int mySize, int* outArray);
int printIntArray(int* array, int length);
double getElapsedTime(struct timespec start, struct timespec finish);
_Bool isInputTwoNumbers(int argc, char **argv);
int checkInput(int argc, char **argv);
double doubleRand(double low, double high);
double** createSquareArray(int size);
double** fillSquareArrayRandomDoubles(double** myArray, int size);
int freeSquareDoubleArray(double** myArray, int size);
double** copySquareDoubleArray(double** myArray, int size);
_Bool isStringAllDigits(char* myString);
void printSquareDoubleArray(double **myArray, int size);
double** createIdentityMatrix(int size);
int testFunctions();
void printListValues(struct node* head);
int getListSize(struct node* head);
struct node* copyNode(struct node* myNode);
void freeList(struct node* head);

//stuff
int runArrayTests(int maxM, int maxN);
int powerArrays(double **myArray, int sizeM, int N);
double **multSquareArrays(double **original, double **intermediate, double **output, int size);
int printArrayDataPoint(int M, int N, int i, int j, double time);
int deleteFileIfExists(char * myFilename);
int runListTests(int maxM, int maxN);
struct node* createList(int size);
struct node* nodeAlloc();
void printNodeValues(struct node* myNode);
struct node* constructNode();
struct node* multTwoSquareLists(struct node* A, struct node* B);
double** fillArrayWithListVals(struct node* head);
struct node* advanceNode(struct node* myNode, int row, int col);
struct node* zeroAList(struct node* myList);
int listDemo(int M, int N);
struct node* powerLists(struct node* myList, int sizeThisRound, int ennThisRound);
struct node* copyList(struct node* myList);
int printListDataPoint(int maxM, int maxN, int size, int ennThisRound, double lowest);
_Bool isInputThreeNumbers(int argc, char **argv);



int testFunctions()
{
	int testSize = 3;
	double **myArray = createSquareArray(testSize);
	myArray = fillSquareArrayRandomDoubles(myArray, testSize);
	printSquareDoubleArray(myArray, testSize);
	double** identity = createIdentityMatrix(testSize);
	printSquareDoubleArray(identity, testSize);
	
	printf("first times identity...\n");
	double **output = createSquareArray(testSize);
	output = multSquareArrays(identity, myArray, output, testSize);
	printSquareDoubleArray(output, testSize);
	
	freeSquareDoubleArray(myArray, testSize);
	freeSquareDoubleArray(identity, testSize);
	freeSquareDoubleArray(output, testSize);

	return 0;
}


//Creates identity matrix of given size. Malloc inide. free outside.
double** createIdentityMatrix(int size)
{
	double** myArray = (double**)malloc( sizeof(double*) * size );
	for (int i = 0; i < size; i++)
	{
		//calloc( #elements, size in bytes), sets all to zero.
		myArray[i] = (double*)calloc( size, sizeof(double) );
		myArray[i][i] = 1;
	}
	
	return myArray;
}


//listMalloc inside, freeList outside
struct node* createIdentityList(int size)
{
	struct node* myList = createList(size);
	
	struct node* i = myList;
	
	while (i != NULL)
	{
		struct node* j = i;
		while (j != NULL)
		{
			if (j->row == j->col)
			{
				j->value = 1;
			}
			else
			{
				j->value = 0;
			}
			j = j->right;
		}
		i = i->down;
	}
	
	return myList;
}

struct node* zeroAList(struct node* myList)
{
	struct node* i = myList;
	
	while (i != NULL)
	{
		struct node* j = i;
		while (j != NULL)
		{
			j->value = 0;
			j = j->right;
		}
		i = i->down;
	}
	
	return myList;
}


int listDemo(int M, int N)
{
	//
	int size = M;
	struct node* head = createList(size);
	freeList(head);
	
	struct node* A = createList(size);
	struct node* B = createList(size);
	//~ struct node* B = createIdentityList(size); //to see if it works!
	struct node* output = multTwoSquareLists(A, B);
	
	freeList(A);
	freeList(B);
	freeList(output);
	
	return 0;
}

int runListTests(int maxM, int maxN)
{
	//~ listDemo(maxM, maxN);
	
	struct timespec start, finish;
	double lowest = INT_MAX;
	double elapsedTime = -1;
	
	for (int i = 0; i < maxM; i++)
	{
		for (int j = 0; j < maxN; j++)
		{
			//~ int sizeThisRound = i;
			int ennThisRound = j;
			
			//do three tests timing each
			lowest = INT_MAX;
			for (int k = 0; k < 3; k++)
			{
				clock_gettime(CLOCK_MONOTONIC, &start);
				
				int size = i;
				struct node* A = createList(size);
				struct node* B = createList(size);
				
				for (int hey = 0; hey < ennThisRound - 1; hey++)
				{
					struct node* output = multTwoSquareLists(A, B);
					freeList(output);
				}
				elapsedTime = getElapsedTime(start, finish);
				
				freeList(A);
				freeList(B);
				
				//~ powerLists(myList, sizeThisRound, ennThisRound); //
				if (elapsedTime < lowest)
				{
					lowest = elapsedTime;
				}
				
				printListDataPoint(maxM, maxN, size, ennThisRound, lowest);
			}
		}
	}
	
	return 0;
}


int printListDataPoint(int M, int N, int i, int j, double time)
{
	char dataFolder[] = "../data/";
	char dataBackupName[1000];
	sprintf(dataBackupName, "%slab1-CLISTSdata-M%d-N%d.txt", dataFolder, M, N);
	
	
	FILE *myFile = fopen(dataBackupName, "a");
	fprintf(myFile, "%d %d %f\n", i, j, time);
	fclose(myFile);
	
	return 0;
}

//creates intermediate with maloc( returns intermediate) free list outside.
struct node* powerLists(struct node* myList, int size, int N)
{
	printf("POWER LIST!\n");
	//~ struct node* original = copyList(myList);
	struct node* intermediate = copyList(myList);
	//~ intermediate = zeroAList(intermediate);
	
	for (int i = 1; i < N; i++)
	{
		struct node* output = createList(size);
		//~ output = multTwoSquareLists(original, intermediate);
		output = multTwoSquareLists(myList, myList);

		//copy output into intermediate
		intermediate = copyList(output);
		
		freeList(output);
	}
	
	printListValues(intermediate);
	
	//~ freeSquareDoubleArray(intermediate, sizeM);
	//~ freeSquareDoubleArray(originalCopy, sizeM);
	
	
	return myList;
}


struct node* copyList(struct node* myList)
{
	//create an array to hold the nodes' addresses for linking.
	int size = getListSize(myList);
	struct node* reference[size][size];
	
	struct node* inode = myList;
	
	int i = 0;
	while (inode != NULL)
	{
		struct node* jnode = inode;
		int j = 0;
		while (jnode != NULL)
		{
			reference[i][j] = constructNode();
			reference[i][j]->row = i;
			reference[i][j]->col = j;
			reference[i][j]->value = jnode->value;
			jnode = jnode->right;
		}
		inode = inode->down;
	}
	
	//link nodes.
	for (i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			//there might be a more clever way to do this.
			if (i != size - 1)
			{
				reference[i][j]->down = reference[i+1][j];
			}
			if (j != size - 1)
			{
				reference[i][j]->right = reference[i][j+1];
			}
		}
	}
	
	struct node* head = reference[0][0];
	
	//~ printListValues(head);
	
	return head;
}

//Multiplies two SQUARE lists. Returns pointer to its head. Uses malloc.
//free with freeList.
struct node* multTwoSquareLists(struct node* A, struct node* B)
{
	int size = getListSize(A);
	struct node* output = createList(size);
	output = zeroAList(output);
	//~ printf("print output after creation:\n");
	//~ printListValues(output);
	//~ double** aVectors = createSquareArray(size);
	//~ double** bVectors = createSquareArray(size);
	
	//fill vectors with values from lists in case I need them.
	//~ aVectors = fillArrayWithListVals(A);
	//~ bVectors = fillArrayWithListVals(B);

	struct node* tA = A;
	struct node* tB = B;
	struct node* tO = output;
	//~ int iCount = 0;
	
	//if current row/col wrong, advance.
	for (int i = 0; i < size; i++)
	{
		//reset heads each loop.
		tA = A;
		tB = B;
		tO = output;
		for (int j = 0; j < size; j++)
		{
			tA = A;
			tB = B;
			tO = output;
			tO = advanceNode(tO, i, j);
			for (int k = 0; k < size; k++)
			{
				//~ tA = A;
				//~ tB = B;
				tA = advanceNode(tA, i, k);
				tB = advanceNode(tB, k, j);
				
				tO->value += (tA->value * tB->value);
			}
		}
	}
	
	//~ printSquareDoubleArray(aVectors, size);
	//~ printSquareDoubleArray(bVectors, size);
	//~ printf("printing list output: \n");
	//~ printListValues(output);
	
	//~ //fill b vectors
	
	//~ //free stuff
	//~ freeSquareDoubleArray(aVectors, size);
	//~ freeSquareDoubleArray(bVectors, size);
	
	return output;
}

struct node* advanceNode(struct node* myNode, int row, int col)
{
	while (myNode->row < row)
	{
		myNode = myNode->down;
	}
	
	while (myNode->col < col)
	{
		myNode = myNode->right;
	}
	
	return myNode;
}



void printListValues(struct node* head)
{
	struct node* i = head;
	
	while (i != NULL)
	{
		struct node* j = i;
		while (j != NULL)
		{
			printf("%f", j->value);
			j = j->right;
			if (j)
			{
				printf("\t");
			}
		}
		printf("\n");
		i = i->down;
	}
	printf("\n");
}


//uses malloc.
double** fillArrayWithListVals(struct node* head)
{
	int size = getListSize(head);
	double** myArray = createSquareArray(size);

	struct node* i = head;
	int iCount = 0;
	while (i != NULL)
	{
		int jCount = 0;
		struct node* j = i;
		while (j != NULL)
		{
			myArray[iCount][jCount] = j->value;
			j = j->right;
			jCount++;
		}
		i = i->down;
		iCount++;
	}
	return myArray;
}

//creates the list, returns pointer to head. malloc used. must free.
struct node* createList(int size)
{
	//create an array to hold the nodes' addresses for linking.
	struct node* reference[size][size];
	
	//create nodes and fill with values.
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			reference[i][j] = constructNode();
			reference[i][j]->row = i;
			reference[i][j]->col = j;
			//~ printNodeValues(reference[i][j]);
		}
	}
	
	//link nodes.
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			//there might be a more clever way to do this.
			if (i != size - 1)
			{
				reference[i][j]->down = reference[i+1][j];
			}
			if (j != size - 1)
			{
				reference[i][j]->right = reference[i][j+1];
			}
		}
	}
	
	struct node* head = reference[0][0];
	
	//~ printListValues(head);
	
	return head;
}


void freeList(struct node* head)
{
	struct node* currentRow = head;
	//~ int count = 0;
	while (currentRow != NULL)
	{
		struct node* currentCol = currentRow;
		while ( currentCol != NULL )
		{
			struct node* tmp = currentCol;
			currentCol = currentCol->right;
			//~ printf("value of thing being freed: %f\n", tmp->value);
			free(tmp);
			//~ count++;
		}
		currentRow = currentRow->down;
	}
	//~ printf("number of frees: %d\n", count);
}




//returns the size of the list as if it were a square array. size x size
int getListSize(struct node* myNode)
{
	struct node* tmp = myNode;
	int count = 0;
	while (tmp != NULL)
	{
		count++;
		tmp = tmp->right;
	}
	
	return count;
}


//creates and returns an elementwise copy of a node. uses malloc.
struct node* copyNode(struct node* myNode)
{
	struct node* copy = nodeAlloc();
	copy->col = myNode->col;
	copy->row = myNode->row;
	copy->right = myNode->right;
	copy->down = myNode->down;
	copy->value = myNode->value;
	
	return copy;
}

//Creates a node. Malloc inside.
struct node* constructNode()
{
	struct node* myNode = nodeAlloc();
	myNode->value = doubleRand(-1, 1) * RAND_MULT;
	myNode->row = -1;
	myNode->col = -1;
	myNode->right = NULL;
	myNode->down = NULL;
	
	return myNode;
}

void printNodeValues(struct node* N)
{
	printf("\nNode %d,%d...\n", N->row, N->col);
	printf("This address: %p\n", N);
	printf("value: %f\n", N->value);
	printf("addr right: %p\taddr down: %p\n", N->right, N->down);
}

//mallocs space for a node. free outside.
struct node* nodeAlloc()
{
	return (struct node*)malloc( sizeof(struct node) );
}

int deleteFileIfExists(char * myFilename)
{
	if ( access(myFilename, F_OK) == 0 )
	{
		//exists
		char command[MAX_COMMAND_LEN];
		sprintf(command, "rm %s", myFilename);
		printf("deleting file: %s\n", myFilename);
		system( command );
	}
	else
	{
		printf("could not find file: %s\n", myFilename);
	}
	
	return 0;
}

int runArrayTests(int maxM, int maxN)
{
	//create timer variables.
	struct timespec start, finish;
	double lowest = INT_MAX;
	double elapsedTime = -1;
	for (int i = 1; i < maxM + 1; i++)
	{
		for (int j = 1; j < maxN + 1; j++)
		{
			//create array
			int sizeThisRound = i;
			int ennThisRound = j;
			double **myArray = createSquareArray(sizeThisRound);
			myArray = fillSquareArrayRandomDoubles(myArray, sizeThisRound);
			
			//do three tests timing each
			lowest = INT_MAX;
			for (int k = 0; k < 3; k++)
			{
				clock_gettime(CLOCK_MONOTONIC, &start);
				powerArrays(myArray, sizeThisRound, ennThisRound);
				elapsedTime = getElapsedTime(start, finish);
				if (elapsedTime < lowest)
				{
					lowest = elapsedTime;
				}
			}

			//append the i, j, time to a file
			printArrayDataPoint(maxM, maxN, sizeThisRound, ennThisRound, lowest);
			
			freeSquareDoubleArray(myArray, sizeThisRound);
		}
	}
	
	
	return 0;
}


int freeSquareDoubleArray(double** myArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		free(myArray[i]);
	}
	free(myArray);
	
	return 0;
}

//Creates square array. Returns pointer to it. Must free outside.
double** createSquareArray(int size)
{
	double **myArray = (double**)malloc( sizeof(double*) * size );
	for (int i = 0; i < size; i++)
	{
		myArray[i] = (double*)malloc( sizeof(double) * size );
	}
	
	return myArray;
}


//fills a square array with random double values.
//returns pointer to array, lol
double** fillSquareArrayRandomDoubles(double** myArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			myArray[i][j] = doubleRand(-1, 1) * RAND_MULT;
		}
	}
	
	return myArray;
}

//need to delete file at start of experiment if it exists.
int printArrayDataPoint(int M, int N, int i, int j, double time)
{
	//~ dataFolder = "../data/"
	//~ dataBackupName = f"lab1-data-M{maxSquareSize}-N{maxN}.txt"
	//~ dataBackupNamePath = dataFolder + dataBackupName
	
	char dataFolder[] = "../data/";
	char dataBackupName[1000];
	sprintf(dataBackupName, "%slab1-CARRAYdata-M%d-N%d.txt", dataFolder, M, N);
	
	
	FILE *myFile = fopen(dataBackupName, "a");
	fprintf(myFile, "%d %d %f\n", i, j, time);
	fclose(myFile);
	
	return 0;
}

int powerArrays(double **myArray, int sizeM, int N)
{
	double **originalCopy = copySquareDoubleArray(myArray, sizeM);
	double **intermediate = copySquareDoubleArray(myArray, sizeM);
	
	for (int i = 1; i < N; i++)
	{
		double **output = createSquareArray(sizeM);
		output = multSquareArrays(originalCopy, intermediate, output, sizeM);

		//copy output into intermediate
		intermediate = copySquareDoubleArray(output, sizeM);
		
		freeSquareDoubleArray(output, sizeM);
	}
	
	freeSquareDoubleArray(intermediate, sizeM);
	freeSquareDoubleArray(originalCopy, sizeM);
	
	return 0;
}

//Creates a copy of a square double array. Must be freed outside.
double** copySquareDoubleArray(double** myArray, int size)
{
	double **myCopy = (double**)malloc( sizeof(double*) * size );
	for (int i = 0; i < size; i++)
	{
		myCopy[i] = (double*)malloc( sizeof(double) * size );
		for (int j = 0; j < size; j++)
		{
			myCopy[i][j] = myArray[i][j];
		}
		
	}
	
	return myCopy;
}

double **multSquareArrays(double **original, double **intermediate, double **output, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				//outArray[i,j] += A[i,k] * B[k,j]
				output[i][j] += original[i][k] * intermediate[k][j];
			}
		}
	}
	
	return output;
}


//Simple random double function that I found. rand/max gives a fraction
//which is multiplied by the range.
double doubleRand(double low, double high)
{
    return ( (double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
}


int checkInput(int argc, char **argv)
{
	//~ if ( !isInputTwoNumbers(argc, argv) )
	if ( !isInputThreeNumbers(argc, argv) )
	{
		printf("Input must be three positive integers!\n");
		printTextFile(HELP_TEXT_PATH);
		exit( -1);
	}
	//I can add more checks for isDigit and such here.
	
	return 0;
}

_Bool isInputTwoNumbers(int argc, char **argv)
{
	if (argc != 3)
	{
		return FALSE;
	}
	else
	{
		for (int i = 1; i < argc; i++) //Start at 1 to skip name.
		{
			//if any character not digit return false.
			if ( !isStringAllDigits(argv[i]) )
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

_Bool isInputThreeNumbers(int argc, char **argv)
{
	if (argc != 4)
	{
		return FALSE;
	}
	else
	{
		for (int i = 1; i < argc; i++) //Start at 1 to skip name.
		{
			//if any character not digit return false.
			if ( !isStringAllDigits(argv[i]) )
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;
}


//Returns TRUE if all characters are digits.
_Bool isStringAllDigits(char* myString)
{
	int c = 0;
	while (myString[c] != 0)
	{
		if ( !isdigit(myString[c]) )
		{
			return FALSE;
		}
		
		c++;
	}
	
	return TRUE;
}

//Timer function. Returns elapsed time.
double getElapsedTime(struct timespec start, struct timespec finish)
{
    double timeElapsed = 0;
    clock_gettime(CLOCK_MONOTONIC, &finish);//end
    timeElapsed = (finish.tv_sec - start.tv_sec);
    timeElapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; //convert to nanoseconds

    return timeElapsed;
}


//Check if user needs help.
int checkForHelpCommands(int argc, char** argv)
{
	if ( ((argc > 1) && strIsHelp(argv[1])) || (argc == 1) )
	{
		printTextFile(HELP_TEXT_PATH);
		
		exit(0);
	}
	
	return 0;
}


//Returns true if input string is a help command.
_Bool strIsHelp(char* aString)
{
	if ( (strcmp(aString, "help") == 0)
			|| (strcmp(aString, "-help") == 0)
			|| (strcmp(aString, "--help") == 0)
			|| (strcmp(aString, "h") == 0)
			|| (strcmp(aString, "-h") == 0)
			|| (strcmp(aString, "--h") == 0)
	)
	{
		return TRUE;
	}
	
	return FALSE;
}


//print a text file. Input string of the filename-path
void printTextFile(char* inputPath)
{
	FILE* myFile = fopen(inputPath, "r");
	
	int c; //int because EOF is usually negative.
	if ( myFile )
	{
		while ( (c = getc(myFile)) != EOF )
		{
			putchar(c);
		}
		
		fclose(myFile);
	}
}

void printSquareDoubleArray(double **myArray, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%f", myArray[i][j]);
			if ( j != size - 1 )
			{
				printf("\t");
			}
		}
		printf("\n");
	}
	printf("\n");
}


#endif
