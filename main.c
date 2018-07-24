#include <stdio.h>
#include <stdlib.h>
#include "Exception.h"
#include "Malloc.h"
#include <time.h>





int main(int argc, char const *argv[])
{
	int* v[50];
	int* p;
	int* t;
	int i;
	clock_t Ticks[2];

	AAlgorithm = FIRST_FIT;

    for(i = 0; i < 10; i++)
    {
    	v[i] = myMalloc((1+rand()%30)*sizeof(int));
    }


    for(i = 0; i < 10; ++i)
    {
    	myMallocFree(v[i]);
    }

    Ticks[0] = clock();
	p = myMalloc(15*sizeof(int));
	Ticks[1] = clock();

	t = myMalloc(15*sizeof(int));

	printf("Allocation Time %f s\n", (Ticks[1]-Ticks[0])/(float)CLOCKS_PER_SEC);

	try
	{
		Try_Access(p, 15, 15, i);
		printf("%i\n", i);
	}catch(MemoryException)
	{
		printf("%s, Exception Code: %i,\n\n\n", MemoryExceptionMessage, MemoryException);
		}
	}

	
    
   

	return 0;
}