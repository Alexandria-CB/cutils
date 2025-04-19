#include "cutils/memory.h"
#include <stdlib.h>

cutils_malloc_t cutils_malloc = malloc;
cutils_calloc_t cutils_calloc = calloc;
cutils_free_t cutils_free = free;

void cutils_memory_init(cutils_malloc_t _malloc, cutils_calloc_t _calloc, cutils_free_t _free)
{
    cutils_malloc = _malloc;
    cutils_calloc = _calloc;
    cutils_free = _free;
}
