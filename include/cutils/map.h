#ifndef CUTILS_MAP_H
#define CUTILS_MAP_H

#include <stdlib.h>

struct cutils_map;

typedef size_t (*cutils_map_hash_t)(void *, void *); // key, hash_ud
typedef int (*cutils_map_cmp_t)(void *, void *, void *); // key1, key2, cmp_ud
typedef void (*cutils_map_free_t)(void *, void *, void *); // key, data, free_ud

struct cutils_map * cutils_map_init(size_t num_buckets,
    cutils_map_hash_t _hash, void * hash_ud,
    cutils_map_cmp_t _cmp, void * cmp_ud,
    cutils_map_free_t _free, void * free_ud);

enum cutils_retcode cutils_map_destroy(struct cutils_map * map);

size_t cutils_map_hash_default(void * key, void * hash_ud);
int cutils_map_cmp_default(void * key1, void * key2, void * cmp_ud);
void cutils_map_free_default(void * key, void * data, void * free_ud);

enum cutils_retcode cutils_map_insert(struct cutils_map * map, void * key, void * data);

enum cutils_retcode cutils_map_remove(struct cutils_map * map, void * key, void ** found_key, void ** found_data);

struct cutils_list * cutils_map_keys(struct cutils_map * map);

#endif//CUTILS_MAP_H
