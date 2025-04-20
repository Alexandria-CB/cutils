#include "cutils/map.h"
#include "cutils/memory.h"
#include "cutils/list.h"

#include <string.h>

struct cutils_map_elem {
    void * key;
    void * data;
};

struct cutils_map {
    struct cutils_list ** buckets; // Array of lists of struct cutils_map_elem
    size_t num_buckets;
    cutils_map_hash_t _hash;
    void * hash_ud;
    cutils_map_cmp_t _cmp;
    void * cmp_ud;
    cutils_map_free_t _free;
    void * free_ud;
};

static void free_map_elem(void * data, void * ud)
{
    struct cutils_map * map = ud;
    struct cutils_map_elem * elem = data;

    if (NULL != map->_free)
    {
        map->_free(elem->key, elem->data, map->free_ud);
    }

    cutils_free(data);
}

struct cutils_map * cutils_map_init(size_t num_buckets,
    cutils_map_hash_t _hash, void * hash_ud,
    cutils_map_cmp_t _cmp, void * cmp_ud,
    cutils_map_free_t _free, void * free_ud)
{
    struct cutils_map * map = cutils_calloc(1, sizeof(*map));
    if (NULL == map)
    {
        return NULL;
    }

    map->buckets = cutils_calloc(num_buckets, sizeof(struct cutils_list *));
    if (NULL == map->buckets)
    {
        cutils_free(map);
        return NULL;
    }

    map->num_buckets = num_buckets;

    for (size_t i=0; i<num_buckets; i++)
    {
        map->buckets[i] = cutils_list_init(free_map_elem, map);
        if (NULL == map->buckets[i])
        {
            cutils_map_destroy(map);
            return NULL;
        }
    }

    map->_hash = _hash == NULL ? cutils_map_hash_default : _hash;
    map->hash_ud = hash_ud;
    map->_cmp = _cmp == NULL ? cutils_map_cmp_default : _cmp;
    map->cmp_ud = cmp_ud;
    map->_free = _free == NULL ? cutils_map_free_default : _free;
    map->free_ud = free_ud;

    return map;
}

enum cutils_retcode cutils_map_destroy(struct cutils_map * map)
{
    if (NULL == map)
    {
        return CUTILS_SUCCESS;
    }

    if (NULL != map->buckets)
    {
        for (size_t i=0; i<map->num_buckets; i++)
        {
            cutils_list_destroy(map->buckets[i]);
        }

        cutils_free(map->buckets);
    }

    cutils_free(map);

    return CUTILS_SUCCESS;
}

size_t cutils_map_hash_default(void * key, void * hash_ud)
{
    size_t code = 0;
    size_t bytes_in_code = sizeof(code);

    char * key_str = key;
    size_t key_len = strlen(key_str);

    size_t code_index = 0;
    size_t key_index = 0;

    for (; key_index < key_len; key_index++, code_index++)
    {
        code_index %= bytes_in_code;

        code |= ((unsigned char)key_str[key_index]) << (code_index * 8);
    }

    return code;
}

int cutils_map_cmp_default(void * key1, void * key2, void * cmp_ud)
{
    char * key_str_1 = key1;
    char * key_str_2 = key2;

    return strcmp(key_str_1, key_str_2);
}

void cutils_map_free_default(void * key, void * data, void * free_ud)
{
    return; // Default to free-ing nothing.
}

enum cutils_retcode cutils_map_insert(struct cutils_map * map, void * key, void * data)
{
    size_t code = map->_hash(key, map->hash_ud) % map->num_buckets;
    struct cutils_list * bucket = map->buckets[code];

    struct cutils_list_elem * ins_before = NULL;

    for (struct cutils_list_elem * bucket_elem = cutils_list_head(bucket);
         bucket_elem != NULL;
         bucket_elem = cutils_list_next(bucket_elem))
    {
        struct cutils_map_elem * map_elem = cutils_list_data(bucket_elem);

        int cmp = map->_cmp(key, map_elem->key, map->cmp_ud);

        if (cmp < 0)
        {
            // Check next item
            continue;
        }
        else if (cmp == 0)
        {
            // Already inserted
            return CUTILS_FAILURE;
        }
        else // cmp > 0
        {
            // Greater than, insert before this elem
            ins_before = bucket_elem;
            break;
        }
    }

    struct cutils_map_elem * elem = cutils_calloc(1, sizeof(*elem));
    if (NULL == elem)
    {
        return CUTILS_FAILURE;
    }

    return cutils_list_ins_before(bucket, ins_before, elem);
}

enum cutils_retcode cutils_map_remove(struct cutils_map * map, void * key, void ** found_key, void ** found_data)
{
    size_t code = map->_hash(key, map->hash_ud) % map->num_buckets;

    struct cutils_list * bucket = map->buckets[code];

    struct cutils_list_elem * found_elem = NULL;

    for (struct cutils_list_elem * bucket_elem = cutils_list_head(bucket);
         bucket_elem != NULL;
         bucket_elem = cutils_list_next(bucket_elem))
    {
        struct cutils_map_elem * map_elem = cutils_list_data(bucket_elem);
        if (map_elem == NULL)
        {
            continue;
        }

        if (0 == map->_cmp(key, map_elem->key, map->cmp_ud))
        {
            found_elem = bucket_elem;
            break;
        }
    }

    if (NULL == found_elem)
    {
        return CUTILS_FAILURE;
    }

    struct cutils_map_elem * data = NULL;

    if (cutils_failure(cutils_list_remove(bucket, found_elem, (void **)&data)))
    {
        return CUTILS_FAILURE;
    }

    if (NULL == found_key && NULL == found_data)
    {
        map->_free(data->key, data->data, map->free_ud);
    }
    else
    {
        if (NULL != found_key)
        {
            *found_key = data->key;
        }

        if (NULL != found_data)
        {
            *found_data = data->data;
        }
    }

    cutils_free(data);
    
    return CUTILS_SUCCESS;
}

struct cutils_list * cutils_map_keys(struct cutils_map * map)
{
    if (NULL == map)
    {
        return NULL;
    }

    struct cutils_list * keys = cutils_list_init(NULL, NULL);
    if (NULL == keys)
    {
        return NULL;
    }

    for (size_t i=0; i<map->num_buckets; i++)
    {
        struct cutils_list * bucket = map->buckets[i];

        for (struct cutils_list_elem * bucket_elem = cutils_list_head(bucket);
             bucket_elem != NULL;
             bucket_elem = cutils_list_next(bucket_elem))
        {
            struct cutils_map_elem * map_elem = cutils_list_data(bucket_elem);
            if (NULL == map_elem)
            {
                continue;
            }

            if (cutils_failure(cutils_list_ins_tail(keys, map_elem->key)))
            {
                cutils_list_destroy(keys);
                return NULL;
            }
        }
    }
    
    return keys;
}