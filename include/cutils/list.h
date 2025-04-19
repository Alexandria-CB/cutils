#ifndef CUTILS_LIST_H
#define CUTILS_LIST_H

#include "stdlib.h"

#include "cutils/retcode.h"

struct cutils_list;
struct cutils_list_elem;

typedef void (*cutils_list_free_t)(void *, void *);

/**
 * @brief Allocate and initialize a linked list structure.
 */
struct cutils_list * cutils_list_init(cutils_list_free_t _list_free, void * user_data);

/**
 * @brief Cleanup and deallocate a linked list structure.
 */
enum cutils_retcode cutils_list_destroy(struct cutils_list * list);

struct cutils_list_elem * cutils_list_head(struct cutils_list * list);

struct cutils_list_elem * cutils_list_tail(struct cutils_list * list);

struct cutils_list_elem * cutils_list_index(struct cutils_list * list, size_t index);

enum cutils_retcode cutils_list_ins_after(struct cutils_list * list, struct cutils_list_elem * elem, void * data);

enum cutils_retcode cutils_list_ins_before(struct cutils_list * list, struct cutils_list_elem * elem, void * data);

enum cutils_retcode cutils_list_ins_head(struct cutils_list * list, void * data);

enum cutils_retcode cutils_list_ins_tail(struct cutils_list * list, void * data);

enum cutils_retcode cutils_list_remove(struct cutils_list * list, struct cutils_list_elem * elem, void ** data);

size_t cutils_list_count(struct cutils_list * list);

struct cutils_list_elem * cutils_list_next(struct cutils_list_elem * elem);

struct cutils_list_elem * cutils_list_prev(struct cutils_list_elem * elem);

void * cutils_list_data(struct cutils_list_elem * elem);

#endif//CUTILS_LIST_H
