#ifndef CUTILS_EXCEPTIONS_H
#define CUTILS_EXCEPTIONS_H

#include "cutils/retcode.h"
#include "cutils/memory.h"

#include <stdnoreturn.h>

typedef int cutils_exception_code_t;

static const cutils_exception_code_t cutils_noexcept = 0;
static const cutils_exception_code_t cutils_internal_error = -1;

struct cutils_exception_ctx;

struct cutils_exception_ctx * cutils_exception_ctx_init(void);

enum cutils_retcode cutils_exception_ctx_destroy(struct cutils_exception_ctx * ctx);

cutils_exception_code_t cutils_exception_ctx_try_impl(struct cutils_exception_ctx * ctx);

noreturn void cutils_exception_ctx_raise_impl(struct cutils_exception_ctx * ctx, cutils_exception_code_t except, void * data, cutils_free_t _free);

void * cutils_exception_ctx_get_data(struct cutils_exception_ctx * ctx);

enum cutils_retcode cutils_exception_ctx_free_data(struct cutils_exception_ctx * ctx);

#define cutils_try(ctx) switch(cutils_exception_ctx_try_impl(ctx)) { \
    case cutils_internal_error: \
    (void)cutils_exception_ctx_free_data(ctx); \
    break;\
    case cutils_noexcept:

#define cutils_except(code) break; \
    case code:

#define cutils_except_generic break; \
    default:

#define cutils_end_try break; \
}

#define cutils_raise(ctx, code, data, _free) cutils_exception_ctx_raise_impl(ctx, code, data, _free)

#endif//CUTILS_EXCEPTIONS_H
