#ifndef CUTILS_RETCODE_H
#define CUTILS_RETCODE_H

enum cutils_retcode
{
    CUTILS_FAILURE = 0,
    CUTILS_SUCCESS = 1,
};

#define cutils_success(expr) ((expr) == CUTILS_SUCCESS)
#define cutils_failure(expr) ((expr) != CUTILS_SUCCESS)

#endif//CUTILS_RETCODE_H
