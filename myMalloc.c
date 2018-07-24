#include "Malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <errno.h>
#include <stdatomic.h>

#ifndef HEAP_SIZE 
	#define HEAP_SIZE 33554432u
#endif

#define ALIGN4(x) (((((x) - 1) >> 2) << 2) + 4)

#define foreach(ptr, List) for(ptr = *List; ptr != NULL; ptr = ptr->next)

#define getHeader(ptr) (((Block)ptr)-1);

#define BlockSize(ptr)   (ptr->Info.Field.size)
#define isFreeBlock(ptr) (ptr->Info.Field.free)


typedef struct Block
{
	struct Block* next;

	union
	{
		unsigned int BlockInfo;
		struct 
		{
			unsigned size:31;
			unsigned free:1;
		}Field;
	}Info;
	unsigned int id;
}*Block;

struct 
{
	void* baseHeap;
	Block LastBlockReleased;
	void* CurrentPointerHeap;
	Block* ListBlocks;
}Gerency;


static unsigned int memoryUsage;
static unsigned int numberBlocks;
static unsigned int numberFreeBlocks;
static unsigned int numberFragmentation;
static unsigned int totalAllocatedMemory;
static const unsigned int MAX_FRAGMENTATION_BLOCK = 10U;
enum AllocationAlgorithm AAlgorithm = FIRST_FIT;



static void __attribute__((constructor))InitMalloc(void)
{
	void* response;

	puts("\n\n");

	Gerency.baseHeap = sbrk(0);
	response = sbrk(ALIGN4(HEAP_SIZE));

	totalAllocatedMemory += ALIGN4(HEAP_SIZE);

	assert(!(response == (void*)-1));

	Gerency.ListBlocks = sbrk(0);
	response = sbrk(sizeof(Block));
    
    totalAllocatedMemory += sizeof(Block);


	assert(!(response == (void*)-1));

	*(Gerency.ListBlocks) = NULL;
	Gerency.LastBlockReleased = NULL;
	numberBlocks = 0;
	Gerency.CurrentPointerHeap = Gerency.baseHeap;
}


void* myMalloc(unsigned int size)
{
	Block newBlock;
	Block scroll;
	Block previousScroll;
	Block enjoyBlock;
	Block tempBlock;
	Block* headerList;
	int spaceFree = 0;
	int spaceEvaluation = 0;

	size = ALIGN4(size);

	if((Gerency.CurrentPointerHeap) - (Gerency.baseHeap) == HEAP_SIZE)
		return NULL;

	if(((Gerency.CurrentPointerHeap) + size) >= (void*)HEAP_SIZE)
	{
		return NULL;
	}

	if((*(Gerency.ListBlocks)) == NULL)
	{
		newBlock = Gerency.CurrentPointerHeap;
		newBlock->next = NULL;
		newBlock->Info.Field.free = 0;
		newBlock->Info.Field.size = size;
		newBlock->id = numberBlocks;
		*(Gerency.ListBlocks) = newBlock;
		
		numberBlocks += 1;

		Gerency.CurrentPointerHeap += ((sizeof(struct Block) + size));
		memoryUsage += ((sizeof(struct Block) + size));
	} 
	else
	{
		if((Gerency.LastBlockReleased != NULL) && ((BlockSize((Gerency.LastBlockReleased)) - size) <= MAX_FRAGMENTATION_BLOCK))
		{
			enjoyBlock = Gerency.LastBlockReleased;
			enjoyBlock->Info.Field.free = 0;
			Gerency.LastBlockReleased = NULL;
			numberFragmentation += BlockSize(enjoyBlock) - size;
			numberFreeBlocks -= 1;
			return (enjoyBlock + 1);
		}
		else
		{
			switch(AAlgorithm)
			{
				case FIRST_FIT:
				try_first_fit:


					foreach(enjoyBlock,(Gerency.ListBlocks))
					{
						if((BlockSize(enjoyBlock) >= size) && (isFreeBlock(enjoyBlock)))
						{
							enjoyBlock->Info.Field.free = 0;
							numberFreeBlocks -= 1;
							numberFragmentation += BlockSize(enjoyBlock) - size;
							return (enjoyBlock + 1);
						}
					}

				break;

				case BEST_FIT:


					spaceFree = BlockSize((*(Gerency.ListBlocks))) - size;
					tempBlock = *(Gerency.ListBlocks);

					foreach(enjoyBlock, Gerency.ListBlocks)
					{		
						spaceEvaluation  = BlockSize(enjoyBlock) - size;
					 	if((spaceFree < 0) || ((spaceEvaluation <= spaceFree) && (isFreeBlock(enjoyBlock))))
						{
							spaceFree = spaceEvaluation;
							tempBlock = enjoyBlock;
						}
					}
					if(spaceFree < 0)
					{
						break;
					}
					else if(isFreeBlock(tempBlock))
					{
						tempBlock->Info.Field.free = 0;
						numberFragmentation += BlockSize(tempBlock) - size;
						return (tempBlock + 1);
					}

				break;


				case WORST_FIT:

					spaceFree = BlockSize((*(Gerency.ListBlocks))) - size;
					tempBlock = *(Gerency.ListBlocks);

					foreach(enjoyBlock, (Gerency.ListBlocks))
					{		
						spaceEvaluation  = BlockSize(enjoyBlock) - size;

					 	if((spaceFree < 0) || ((spaceEvaluation > spaceFree) && (isFreeBlock(enjoyBlock))))
						{
							spaceFree = spaceEvaluation;
							tempBlock = enjoyBlock;
						}
					}

					if(isFreeBlock(tempBlock))
					{
						tempBlock->Info.Field.free = 0;
						return (tempBlock + 1);
					}

				break;

				default:
					goto try_first_fit;

			}

			scroll = *(Gerency.ListBlocks);

			while((scroll != NULL) && (BlockSize(scroll) < size))
			{
				previousScroll = scroll;
				scroll = scroll->next;
			}

			newBlock = Gerency.CurrentPointerHeap;
			newBlock->Info.Field.free = 0;
			newBlock->Info.Field.size = size;
			newBlock->id = numberBlocks;
			numberBlocks += 1;

			if(scroll == *(Gerency.ListBlocks) )
			{
				newBlock->next = *(Gerency.ListBlocks);
				*(Gerency.ListBlocks) = newBlock;
			}
			
			else
			{
				newBlock->next = previousScroll->next;
				previousScroll->next = newBlock;
			}

			Gerency.CurrentPointerHeap += ((sizeof(struct Block) + size));
			memoryUsage += ((sizeof(struct Block) + size));
		}
	}

	return (newBlock + 1);
}

void* myCalloc(unsigned int length, unsigned int size)
{

}

int myMallocFree(void* ptr)
{
	Block releaseBlock;

	releaseBlock = getHeader(ptr);
	releaseBlock->Info.Field.free = 1;

	if(Gerency.LastBlockReleased == NULL)
	{
		Gerency.LastBlockReleased = releaseBlock;
	}
	numberFreeBlocks += 1;
}

void __attribute__((destructor))Exit(void)
{
	Block p;

	printf("Allocated memory for heap: %u Bytes , Total Allocated Memory %u Bytes\n\n", HEAP_SIZE, totalAllocatedMemory);
	printf("Memory Usage: %u Bytes \n\n",memoryUsage);
	printf("Internal Fragmentation: %u Bytes \n\n", numberFragmentation);
	
	foreach(p, (Gerency.ListBlocks))
	{
		printf("Block ID: %u ", p->id);
		printf("Size: %u ", BlockSize(p));
		printf("Free: %s\n", isFreeBlock(p)?"True":"False");
		puts(" ");
	}
}