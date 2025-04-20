#include "cutils.h"
#include "test_cutils.h"
#include <stdio.h>

static void test_list_create(void)
{
    struct cutils_list * l = cutils_list_init(NULL, NULL);
    test_assert_true_fatal(l != NULL);
    test_assert_true(cutils_list_destroy(l) == CUTILS_SUCCESS);
}

static void test_list_ins_head(void)
{
    struct cutils_list * l = cutils_list_init(NULL, NULL);
    test_assert_true_fatal(NULL != l);
    
    int a = 1;
    int b = 2;
    int c = 3;

    int expected[] = {3, 2, 1};

    test_assert_true(cutils_list_ins_head(l, &a) == CUTILS_SUCCESS);
    test_assert_true(cutils_list_ins_head(l, &b) == CUTILS_SUCCESS);
    test_assert_true(cutils_list_ins_head(l, &c) == CUTILS_SUCCESS);

    test_assert_true(cutils_list_count(l) == 3);

    int i=0;
    for (struct cutils_list_elem * e = cutils_list_head(l);
         e != NULL;
         e = cutils_list_next(e), i++)
    {
        int * n = cutils_list_data(e);
        test_assert_true(*n == expected[i]);
    }

    test_assert_true(cutils_list_destroy(l) == CUTILS_SUCCESS);
}

static void test_list_ins_tail(void)
{
    struct cutils_list * l = cutils_list_init(NULL, NULL);
    test_assert_true_fatal(NULL != l);
    
    int a = 1;
    int b = 2;
    int c = 3;

    int expected[] = {1, 2, 3};

    test_assert_true(cutils_list_ins_tail(l, &a) == CUTILS_SUCCESS);
    test_assert_true(cutils_list_ins_tail(l, &b) == CUTILS_SUCCESS);
    test_assert_true(cutils_list_ins_tail(l, &c) == CUTILS_SUCCESS);

    test_assert_true(cutils_list_count(l) == 3);

    int i=0;
    for (struct cutils_list_elem * e = cutils_list_head(l);
         e != NULL;
         e = cutils_list_next(e), i++)
    {
        int * n = cutils_list_data(e);
        test_assert_true(*n == expected[i]);
    }

    test_assert_true(cutils_list_destroy(l) == CUTILS_SUCCESS);
}

void test_cutils_list(void)
{
    printf("************************\n");
    printf("* Testing cutils lists *\n");
    printf("************************\n");

    TEST(test_list_create);
    TEST(test_list_ins_head);
    TEST(test_list_ins_tail);
}