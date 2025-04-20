#ifndef TEST_CUTILS_H
#define TEST_CUTILS_H

#include <stdnoreturn.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

static jmp_buf test_env;
static char err_buffer[1024];
static bool err;

#define TEST_START 0
#define TEST_FATAL 1

static void runtest(void(*test)(void), char * test_name)
{
    printf("%s ...", test_name);
    err = false;
    switch(setjmp(test_env))
    {
        case TEST_START:
            test();
            break;
        case TEST_FATAL:
            printf("Fatal error!\n%s\n", err_buffer);
            break;
        default:
            printf("Unhandled test result\n");
            break;
    }
    if (!err)
    {
        printf(" ok\n");
    }
}

static noreturn void fatal_error(char * fmt, ...)
{
    va_list args;
    err = true;
    va_start(args, fmt);
    vsnprintf(err_buffer, sizeof(err_buffer), fmt, args);
    va_end(args);
    longjmp(test_env, TEST_FATAL);
}

static void nonfatal_error(char * fmt, ...)
{
    va_list args;
    err = true;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

static void test_assert(bool expr, char * expr_str, bool fatal, int line, const char * function, const char * file)
{
    if (expr)
    {
        return;
    }
    else if (fatal)
    {
        fatal_error("Error: %s:%d (%s) : %s is not true\n", file, line, function, expr_str);
    }
    else
    {
        nonfatal_error("Error: %s:%d (%s) : %s is not true\n", file, line, function, expr_str);
    }
}

#define test_assert_true(expr) test_assert(expr, #expr, false, __LINE__, __func__, __FILE__)

#define test_assert_true_fatal(expr) test_assert(expr, #expr, true, __LINE__, __func__, __FILE__)

#define TEST(func) runtest(func, #func)

typedef void (*unit_test_t)(void);

// Test entry points
void test_cutils_list(void);

static unit_test_t tests[] = {
    test_cutils_list,
    NULL
};

#endif//TEST_CUTILS_H
