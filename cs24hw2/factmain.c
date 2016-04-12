#include <stdio.h>
#include <stdlib.h>
#include "fact.h"

int main(int argc, char **argv)
{
    int input;
    int result;
    if (argc == 2)
    {
        // Number of arguments = 1
        input = atoi(argv[1]);
        if (input < 0)
        {
            // Argument is negative
            fprintf(stderr, "ERROR: Integer must be nonnegative\n");
            fprintf(stderr, "usage: factmain int_x\n");
            fprintf(stderr, "   int_x: a nonnegative integer\n");
            return -1;

        }
    }
    else
    {
        // Number of arguments != 1
        fprintf(stderr, "ERROR: Program must receive exactly one integer\n");
        fprintf(stderr, "usage: factmain int_x\n");
        fprintf(stderr, "   int_x: a nonnegative integer\n");
        return -1;
    }
    result = fact(input);
    printf("%d! = %d\n", input, result);
    return 1;
}