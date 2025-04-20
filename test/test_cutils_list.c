#include "cutils.h"
#include "test_cutils.h"
#include <stdio.h>

void test_list_create(void)
{
    struct cutils_list * l = cutils_list_init(NULL, NULL);
    test_assert_true_fatal(l != NULL);
    test_assert_true(cutils_list_destroy(l) == CUTILS_SUCCESS);
}

void test_cutils_list(void)
{
    printf("************************\n");
    printf("* Testing cutils lists *\n");
    printf("************************\n");

    TEST(test_list_create);
}