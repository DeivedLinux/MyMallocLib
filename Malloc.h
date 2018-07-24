#ifndef MALLOC_H_
#define MALLOC_H_ 

#include <stdio.h>
#include <stdlib.h>
#include "Exception.h"
#include <stdbool.h>



#ifndef ACCESS_CONTROL_BOUND 
	#warning "Transhipment may override the header of another segment"
#else 
	#define MemoryException 45u
	#define MemoryExceptionMessage "The overflow canceled the header of another segment"
	#define Access(local, index, size, res, rw)             	\
	{                                                           \
		if(index >= size) 										\
		{														\
			throw(MemoryException);								\
		} 														\
		else if(rw == false)									\
		{														\
			res = local[index];									\
		}														\
		else													\
		{														\
			local[index] = res;									\
		}														\
	}			

	#define Write(local, index, size, value)             		\
	{                                                           \
		if(index >= size) 										\
		{														\
			throw(MemoryException);								\
		} 														\
		else													\
		{														\
			local[index] = value;								\
		}														\
	}	

	#define Read(local, index, size, value)             		\
	{                                                           \
		if(index >= size) 										\
		{														\
			throw(MemoryException);								\
		} 														\
		else													\
		{														\
			value = local[index];								\
		}														\
	}															
#endif


extern enum AllocationAlgorithm
{
	FIRST_FIT,
	BEST_FIT,
	WORST_FIT
}AAlgorithm;



void* myMalloc(unsigned int size); 
void* myCalloc(unsigned int length, unsigned int size);
int myMallocFree(void* ptr);




#endif