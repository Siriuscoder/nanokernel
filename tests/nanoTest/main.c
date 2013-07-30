/* 
 * File:   main.c
 * Author: sirius
 *
 * Created on April 3, 2013, 11:22 PM
 */

#include <stdio.h>
#include <stdlib.h>


#ifdef NULL
#undef NULL
#endif

#define TEST_FAILED(x)  \
    { printf("%s TEST FAILED at %s:%d\n", #x, __FILE__, __LINE__); \
    return EXIT_FAILURE; }

#define TEST_OK(x)  \
    { printf("%s TEST OK\n", #x); }

#define TEST_DECLARE(x)  if(!x()) TEST_FAILED(x) else TEST_OK(x)


#include <mem.h>

#include "vfs_test.h"

/*
 * 
 */
int
main(int argc, char** argv)
{
    TEST_DECLARE(k_heap_init);
	TEST_DECLARE(kmalloc_test);
    TEST_DECLARE(run_vfs_test);
    
    k_print_memory_usage_info();
    
    return EXIT_SUCCESS;
}

