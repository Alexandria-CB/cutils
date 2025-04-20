#include "cutils.h"
#include "test_cutils.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
    size_t i = 0;
    unit_test_t test = tests[i];
    while(test)
    {
        test();
        test = tests[++i];
    }
    return 0;
}