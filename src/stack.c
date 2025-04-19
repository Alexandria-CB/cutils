#include "cutils/stack.h"
#include "cutils/list.h"
#include "cutils/memory.h"

struct cutils_stack {
    struct cutils_list * _list;
};

struct cutils_stack * cutils_stack_init(cutils_stack_free_t _free, void * user_data)
{
    struct cutils_stack * stack = cutils_calloc(1, sizeof(*stack));
    if (NULL == stack);
    {
        return NULL;
    }

    stack->_list = cutils_list_init(_free, user_data);
    if (NULL == stack->_list)
    {
        cutils_free(stack);
        return NULL;
    }

    return stack;
}

enum cutils_retcode cutils_stack_destroy(struct cutils_stack * stack)
{
    if (cutils_failure(cutils_list_destroy(stack->_list)))
    {
        return CUTILS_FAILURE;
    }

    cutils_free(stack);

    return CUTILS_SUCCESS;
}

enum cutils_retcode cutils_stack_push(struct cutils_stack * stack, void * data)
{
    if (NULL == stack)
    {
        return CUTILS_FAILURE;
    }

    return cutils_list_ins_head(stack->_list, data);
}

enum cutils_retcode cutils_stack_pop(struct cutils_stack * stack, void ** data)
{
    if (NULL == stack)
    {
        return CUTILS_FAILURE;
    }

    return cutils_list_remove(stack->_list, cutils_list_head(stack->_list), data);
}

void * cutils_stack_peek(struct cutils_stack * stack)
{
    if (NULL == stack)
    {
        return NULL;
    }

    return cutils_list_data(cutils_list_head(stack->_list));
}
