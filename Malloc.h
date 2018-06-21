#ifndef MALLOC
#define MALLOC 


#ifndef SIZE_MEMORY
	#define SIZE_MEMORY 2e27ul
#endif


void* __attribute__((weak)) mMalloc(unsigned long size);
void __attribute__((weak)) 	mCalloc(unsigned long length, unsigned long cel);
void __attribute__((weak))  mFree(void* ptr);
void __attribute__((weak)) 	mMallocGerency(void);



#endif