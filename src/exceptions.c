#include "cutils/exceptions.h"
#include "cutils/stack.h"
#include "cutils/memory.h"

#include <string.h>
#include <setjmp.h>

struct cutils_exception_ctx {
    struct cutils_stack * env_stack; // Stack of struct cutils_exception_ctx_elem
    void * data; // Current data
    cutils_free_t _free; // Current free handler for data
};

static void free_env(void * data, void * ud)
{
    cutils_free(data);
}

struct cutils_exception_ctx * cutils_exception_ctx_init(void)
{
    struct cutils_exception_ctx * ctx = cutils_calloc(1, sizeof(*ctx));
    if (NULL == ctx)
    {
        return NULL;
    }

    ctx->env_stack = cutils_stack_init(free_env, NULL);
    if (NULL == ctx->env_stack)
    {
        cutils_free(ctx);
        return NULL;
    }

    return ctx;
}

enum cutils_retcode cutils_exception_ctx_destroy(struct cutils_exception_ctx * ctx)
{
    cutils_stack_destroy(ctx->env_stack);
    if (NULL != ctx->_free)
    {
        ctx->_free(ctx->data);
    }

    cutils_free(ctx);

    return CUTILS_SUCCESS;
}

cutils_exception_code_t cutils_exception_ctx_try_impl(struct cutils_exception_ctx * ctx)
{
    jmp_buf * env = cutils_calloc(1, sizeof(*env));
    if (NULL == env)
    {
        return cutils_internal_error;
    }

    if (cutils_failure(cutils_stack_push(ctx->env_stack, env)))
    {
        cutils_free(env);
        return cutils_internal_error;
    }

    return setjmp(env);
}

noreturn void cutils_exception_ctx_raise_impl(struct cutils_exception_ctx * ctx, cutils_exception_code_t except, void * data, cutils_free_t _free)
{
    jmp_buf * env;
    jmp_buf local = { 0 };
    (void)cutils_stack_pop(ctx->env_stack, (void **)&env);
    ctx->data = data;
    ctx->_free = _free;
    memcpy(&local, env, sizeof(*local));
    cutils_free(env);
    longjmp(local, except);
}

void * cutils_exception_ctx_get_data(struct cutils_exception_ctx * ctx)
{
    return ctx->data;
}

enum cutils_retcode cutils_exception_ctx_free_data(struct cutils_exception_ctx * ctx)
{
    if (NULL != ctx->_free)
    {
        ctx->_free(ctx->data);
    }

    return CUTILS_SUCCESS;
}
