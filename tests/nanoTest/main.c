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
    { printf("%s TEST FAILED at %s:%d", #x, __FILE__, __LINE__); \
    return EXIT_FAILURE; }

#define TEST_OK(x)  \
    { printf("%s TEST OK", #x); }

#define TEST_DEFINE(x)  if(!x()) TEST_FAILED(x) else TEST_OK(x)


#include <mem.h>

/*
 * 
 */
int
main(int argc, char** argv)
{
    TEST_DEFINE(k_heap_init)

    return EXIT_SUCCESS;
}

