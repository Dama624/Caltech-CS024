#include <stdio.h>
#include <stdlib.h>
#include "gcd.h"

int main(int argc, char **argv)
{
    int input1;
    int input2;
    int result;
    if (argc == 3)
    {
        // Number of arguments = 2
        input1 = atoi(argv[1]);
        input2 = atoi(argv[2]);
        if (input1 <= 0 || input2 <= 0)
        {
            // Argument is <= 0
            fprintf(stderr, "ERROR: Integers must be positive\n");
            fprintf(stderr, "usage: gcdmain int_a int_b\n");
            fprintf(stderr, "   int_a: a positive integer\n");
            fprintf(stderr, "   int_b: a positive integer\n");
            return -1;
        }
    }
    else
    {
        // Number of arguments != 2
        fprintf(stderr, "ERROR: Program must receive exactly two integers\n");
        fprintf(stderr, "usage: gcdmain int_a int_b\n");
        fprintf(stderr, "   int_a: a positive integer\n");
        fprintf(stderr, "   int_b: a positive integer\n");
        return -1;
    }
    result = gcd(input1, input2);
    printf("gcd(%d, %d) = %d\n", input1, input2, result);
    return 1;
}