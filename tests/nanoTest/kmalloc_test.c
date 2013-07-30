#include <mem.h>

#include "kmalloc_test.h"

bool kmalloc_test()
{
	char *ptrs[20];
	int i;
	size_t acSize = 10;
	for(i = 0; i < (sizeof(ptrs)/sizeof(char*)); i++, acSize *= 2)
	{
		ptrs[i] = k_malloc(acSize);
		if(!ptrs[i])
			return false;
	}
	
	k_print_memory_usage_info();
	for(i = 0; i < (sizeof(ptrs)/sizeof(char*)); i++, acSize *= 2)
		k_free(ptrs[i]);
	/* mast be empty */
	k_print_memory_usage_info();
	
	acSize = 10;
	for(i = 0; i < (sizeof(ptrs)/sizeof(char*)); i++, acSize *= 2)
	{
		if(i % 2)
		{
			ptrs[i] = k_malloc_aligned(acSize, 4);
			if((((size_t)ptrs[i]) % 4) != 0)
				return false;
		}
		else
		{
			ptrs[i] = k_malloc_aligned(acSize, 8);
			if((((size_t)ptrs[i]) % 8) != 0)
				return false;
		}
	}
	
	k_print_memory_usage_info();
	for(i = 0; i < (sizeof(ptrs)/sizeof(char*)); i++, acSize *= 2)
		k_free(ptrs[i]);
	
	/* mast be empty */
	k_print_memory_usage_info();
	return true;
}