#ifndef CUTILS_MEMORY_H
#define CUTILS_MEMORY_H

#include <stdlib.h>

typedef void *(*cutils_malloc_t)(size_t);
typedef void *(*cutils_calloc_t)(size_t, size_t);
typedef void(*cutils_free_t)(void *);

extern cutils_malloc_t cutils_malloc;
extern cutils_calloc_t cutils_calloc;
extern cutils_free_t cutils_free;

/**
 * @brief Set the memory allocation and free functions to be used by the cutils library
 */
void cutils_init(cutils_malloc_t _malloc, cutils_calloc_t _calloc, cutils_free_t _free);

#endif//CUTILS_MEMORY_H
