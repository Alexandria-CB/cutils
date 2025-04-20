#include "cutils/list.h"
#include "cutils/memory.h"

struct cutils_list_elem {
    struct cutils_list_elem * next;
    struct cutils_list_elem * prev;
    void * data;
};

struct cutils_list {
    size_t count;
    struct cutils_list_elem * head;
    struct cutils_list_elem * tail;
    cutils_list_free_t _free;
    void * user_data;
};

struct cutils_list * cutils_list_init(cutils_list_free_t _list_free, void * user_data)
{
    struct cutils_list * list = cutils_calloc(1, sizeof(*list));
    if (NULL == list)
    {
        return NULL;
    }

    list->_free = _list_free;
    list->user_data = user_data;

    return list;
}

enum cutils_retcode cutils_list_destroy(struct cutils_list * list)
{
    if (NULL == list)
    {
        return CUTILS_SUCCESS;
    }

    struct cutils_list_elem * elem = cutils_list_head(list);

    while (elem != NULL)
    {
        struct cutils_list_elem * next = elem->next;
        
        if (NULL != list->_free)
        {
            list->_free(elem->data, list->user_data);
        }

        cutils_free(elem);

        elem = next;
    }

    cutils_free(list);

    return CUTILS_SUCCESS;
}

struct cutils_list_elem * cutils_list_head(struct cutils_list * list)
{
    return list != NULL ? list->head : NULL;
}

struct cutils_list_elem * cutils_list_tail(struct cutils_list * list)
{
    return list != NULL ? list->tail : NULL;
}

struct cutils_list_elem * cutils_list_index(struct cutils_list * list, size_t index)
{
    if (NULL == list || index >= list->count)
    {
        return NULL;
    }

    struct cutils_list_elem * found = cutils_list_head(list);
    for (size_t i=0; i<list->count; i++)
    {
        if (index == i)
        {
            return found;
        }

        found = cutils_list_next(found);
    }

    return NULL;
}

enum cutils_retcode cutils_list_ins_after(struct cutils_list * list, struct cutils_list_elem * elem, void * data)
{
    if (NULL == list)
    {
        return CUTILS_FAILURE;
    }

    struct cutils_list_elem * next = cutils_list_next(elem);
    struct cutils_list_elem * prev = elem;

    struct cutils_list_elem * entry = cutils_calloc(1, sizeof(*entry));
    if (NULL == entry)
    {
        return CUTILS_FAILURE;
    }

    entry->data = data;
    entry->next = next;
    entry->prev = prev;

    if (entry->prev != NULL)
    {
        entry->prev->next = entry;
    }
    else
    {
        list->head = entry;
    }

    if (entry->next != NULL)
    {
        entry->next->prev = entry;
    }
    else
    {
        list->tail = entry;
    }

    list->count += 1;

    return CUTILS_SUCCESS;
}

enum cutils_retcode cutils_list_ins_before(struct cutils_list * list, struct cutils_list_elem * elem, void * data)
{
    if (NULL == list)
    {
        return CUTILS_FAILURE;
    }

    struct cutils_list_elem * next = elem;
    struct cutils_list_elem * prev = cutils_list_prev(elem);

    struct cutils_list_elem * entry = cutils_calloc(1, sizeof(*entry));
    if (NULL == entry)
    {
        return CUTILS_FAILURE;
    }

    entry->data = data;
    entry->next = next;
    entry->prev = prev;

    if (entry->prev != NULL)
    {
        entry->prev->next = entry;
    }
    else
    {
        list->head = entry;
    }

    if (entry->next != NULL)
    {
        entry->next->prev = entry;
    }
    else
    {
        list->tail = entry;
    }

    list->count += 1;

    return CUTILS_SUCCESS;
}

enum cutils_retcode cutils_list_ins_head(struct cutils_list * list, void * data)
{
    return cutils_list_ins_before(list, cutils_list_head(list), data);
}

enum cutils_retcode cutils_list_ins_tail(struct cutils_list * list, void * data)
{
    return cutils_list_ins_after(list, cutils_list_tail(list), data);
}

enum cutils_retcode cutils_list_remove(struct cutils_list * list, struct cutils_list_elem * elem, void ** data)
{
    if (NULL == list)
    {
        return CUTILS_FAILURE;
    }

    struct cutils_list_elem * prev = cutils_list_prev(elem);
    struct cutils_list_elem * next = cutils_list_next(elem);

    if (prev != NULL)
    {
        prev->next = next;
    }
    else
    {
        list->head = next;
    }

    if (next != NULL)
    {
        next->prev = prev;
    }
    else
    {
        list->tail = prev;
    }

    list->count -= 1;

    if (NULL != data)
    {
        *data = elem->data;
    }
    else if (NULL != list->_free)
    {
        list->_free(elem->data, list->user_data);
    }

    cutils_free(elem);

    return CUTILS_SUCCESS;
}

size_t cutils_list_count(struct cutils_list * list)
{
    return NULL != list ? list->count : 0;
}

struct cutils_list_elem * cutils_list_next(struct cutils_list_elem * elem)
{
    return NULL != elem ? elem->next : NULL;
}

struct cutils_list_elem * cutils_list_prev(struct cutils_list_elem * elem)
{
    return NULL != elem ? elem->prev : NULL;
}

void * cutils_list_data(struct cutils_list_elem * elem)
{
    return NULL != elem ? elem->data : NULL;
}
