#ifndef CUTILS_STACK_H
#define CUTILS_STACK_H

struct cutils_stack;

typedef void (*cutils_stack_free_t)(void *, void *);

struct cutils_stack * cutils_stack_init(cutils_stack_free_t _free, void * user_data);

enum cutils_retcode cutils_stack_destroy(struct cutils_stack * stack);

enum cutils_retcode cutils_stack_push(struct cutils_stack * stack, void * data);

enum cutils_retcode cutils_stack_pop(struct cutils_stack * stack, void ** data);

void * cutils_stack_peek(struct cutils_stack * stack);

#endif//CUTILS_STACK_H
