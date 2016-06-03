#include <stdio.h>
#include "cmp_swap.h"

/* test: tests compare and swap
 *
 * Input:
 *      None. First tests success case of compare_and_swap. Because the
 *      function modifies the value of target, calling the function again
 *      should return a fail.
 *
 * Output:
 *      None. Will print if successful or failed.
 */
void test()
{
    unsigned int *target;
    unsigned int old;
    unsigned int new;
    unsigned int temp;
    unsigned int result;

    // We need target = old
    temp = 5;
    old = 5;
    new = 3;
    target = &temp;

    printf("testing for success:\n");
    result = compare_and_swap(target, old, new);
    // If successful, result = 1
    if (result == 1)
    {
        printf("compare_and_swap returned correct result!\n");
        printf("expected: 1; returned: %d\n\n", result);
    }
    else
    {
        printf("compare_and_swap did not return correct result!\n");
        printf("expected: 1; returned: %d\n\n", result);
    }
    // If successful, target = new
    if (*target == new)
    {
        printf("target changed correctly!\n");
        printf("expected: %d; returned: %d\n\n", new, *target);
    }
    else
    {
        printf("target didn't change correctly!\n");
        printf("expected: %d; returned: %d\n\n", new, *target);
    }

    printf("testing for fail:\n");
    result = compare_and_swap(target, old, new);
    // If fail, result = 0
    if (!result)
    {
        printf("compare_and_swap returned correct result!\n");
        printf("expected: 0; returned: %d\n\n", result);
    }
    else
    {
        printf("compare_and_swap did not return correct result!\n");
        printf("expected: 0; returned: %d\n\n", result);
    }
    // If fail, target = new
    if (*target == new)
    {
        printf("target remained correctly!\n");
        printf("expected: %d; returned: %d\n\n", new, *target);
    }
    else
    {
        printf("target didn't change correctly!\n");
        printf("expected: %d; returned: %d\n\n", new, *target);
    }
    return;
}


int main()
{
    test();
    return 0;
}