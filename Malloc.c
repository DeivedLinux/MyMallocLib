#include "Malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>


static void* heap = NULL;

typedef struct Block
{
	union
	{

	};
	
};


void __attribute__ ((constructor)) myMallocInit(void)
{
	heap = sbrk(0);
	assert(!(heap == (void*)-1));
}


