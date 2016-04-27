#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "c_except.h"


double read_double() {
    char buf[20];
    char *end;
    double val;

    fgets(buf, sizeof(buf), stdin);
    val = strtod(buf, &end);
    if (end == buf)
        THROW(NUMBER_PARSE_ERROR);

    return val;
}


double divide(double a, double b) {
    if (b == 0)
        THROW(DIVIDE_BY_ZERO);

    return a / b;
}


int main() {
    double n1, n2;

    printf("Tell me two numbers, and I will tell you their quotient!\n");
    
    TRY (
        printf("Enter your first number, the dividend:  ");
        n1 = read_double();
        
        printf("Now enter your second number, the divisor:  ");
        n2 = read_double();

        printf("The quotient of %lg / %lg is:  %lg\n", n1, n2, divide(n1, n2));
    )
    CATCH (NUMBER_PARSE_ERROR,
        printf("Ack!!  I couldn't parse what you entered!\n");
        RETHROW;
    )
    CATCH (DIVIDE_BY_ZERO,
        printf("Ack!!  You entered 0 for the divisor!\n");
    )
    END_TRY;

    return 0;
}

