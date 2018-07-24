#include <stdio.h>
#include <stdlib.h>
#include "Exception.h"
#include "Malloc.h"
#include <time.h>
#include <stdbool.h>
#include <math.h>


static volatile clock_t Ticks[2];

static void merge(int* vector, int start, int middle, int end)
{
	bool isEndFirstVector;
	bool isEndSecondVector;
	unsigned size;
	unsigned startFirstVector;
	unsigned startSecondVector;
	int* vectorAux;
	unsigned i,j,k;

	size = end-start+1;
	isEndFirstVector = false;
	isEndSecondVector = false;
	startFirstVector = start;
	startSecondVector = middle+1;

	//Ticks[0] = clock();
	//vectorAux = (int*)malloc(size*sizeof(int));
	vectorAux = (int*)myMalloc(size*sizeof(int));
	//Ticks[1] = clock();

	//printf("Allocation time: %lf s\n",(Ticks[1]-Ticks[0])/(double)CLOCKS_PER_SEC);

	if(vectorAux != NULL)
	{
		for(i = 0; i < size; i++)
		{
			if(!isEndFirstVector && !isEndSecondVector)
			{
				if(vector[startFirstVector] < vector[startSecondVector])
				{
					vectorAux[i] = vector[startFirstVector];
					startFirstVector += 1;
				}
				else 
				{
					vectorAux[i] = vector[startSecondVector];
					startSecondVector += 1;
				}
				if(startFirstVector > middle)
					isEndFirstVector = true;

				if(startSecondVector > end)
					isEndSecondVector = true;
			}
			else
			{
				if(!isEndFirstVector)
				{
					vectorAux[i] = vector[startFirstVector];
					startFirstVector += 1;
				}
				else
				{
					vectorAux[i] = vector[startSecondVector];
					startSecondVector += 1;
				}
			}
		}
		for(j = 0, k = start; j < size; j++,k++)
			vector[k] = vectorAux[j];
	}

	myMallocFree(vectorAux);
	//free(vectorAux);
}

void mergeSort(int* vector, int start, int end)
{
	int middle;

	if(start < end)
	{
		middle = ((start+end)/2);
    	mergeSort(vector,start,middle);
    	mergeSort(vector,middle+1,end);
    	merge(vector,start,middle,end);
    }
}




int main(int argc, char const *argv[])
{
	int* v;
	unsigned i;
	int randValue;

	AAlgorithm = FIRST_FIT;

	v = (int*)myMalloc(100000*sizeof(int));

	if(v == NULL)
	{
		puts("has not memory enoughh");
		return 0;
	}

	for(i = 0; i < 100000; i++)
	{
		try
		{	randValue = rand()%100;
			Write(v, i, 100000, randValue);
			throw(42);
		}catch(MemoryException){

			printf("%s Exception Code: %i\n\n", MemoryExceptionMessage, MemoryException);

			}
		}
	}

	Ticks[0] = clock();
    mergeSort(v,0,100000);
	Ticks[1] = clock();

	printf("Allocation time: %lf s\n",(Ticks[1]-Ticks[0])/(double)CLOCKS_PER_SEC);
	
	
	return 0;
}