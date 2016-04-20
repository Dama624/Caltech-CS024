/********************************************/
/* CS24 RLE Decoder Tests - Source Code     */
/* Donnie Pinkston <donnie@cms.caltech.edu> */
/*                                          */
/* Copyright (C) 2011, Caltech              */
/********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rl_decode.h"


/*!
 * This struct is used to represent a single test-case for exercising
 * the RLE decoder.
 */
typedef struct test_case {
    char encoded_str[100];
    char *decoded_str;
} test_case;


/*!
 * A sequence of simple test cases to verify that the RLE decoder works
 * with various kinds of inputs.
 */
test_case tests[] = {

    /* Simple tests */

    { {1, 'A', 1, 'B', 1, 'C', 1, 'D', 1, 'E', 0},
      "ABCDE"
    },

    { {3, 'A', 2, 'B', 5, 'C', 1, 'D', 3, 'E', 0},
      "AAABBCCCCCDEEE"
    },

    { "", "" },         /* Empty input should produce empty output. */

    /* These cases check overall results that are larger than 255 chars. */

    { {83, 'C', 25, 'B', 17, 'D', 4, 'A', 29, 'D', 106, 'B', 38, 'F'},
      "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
      "CCCCCCCCCCCCC"
      "BBBBBBBBBBBBBBBBBBBBBBBBB"
      "DDDDDDDDDDDDDDDDD"
      "AAAA"
      "DDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
      "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
      "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
      "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
    },

    /* These cases check runs longer than 255 characters, which must be
     * represented as multiple runs of 255 or less characters.
     */

    { {255, 'A', 122, 'A', 0},
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAA"
    },

    { {200, 'A', 177, 'A', 0},
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAA"
    },

    /* Indicates the end of the test sequence. */
    { {0}, NULL}
};


/*!
 * This helper function prints out the specified buffer.  Non-printable
 * characters are shown as "[0x00]" hexadecimal values.
 */
void print_buf(char *buf, int length) {
    int i;
    for (i = 0; i < length; i++) {
        char ch = buf[i];
        if (isprint(ch))
            printf("%c", ch);
        else
            printf("[0x%02X]", ch);
    }
}


/*!
 * Main entry-point for testing that the RLE decoder works.
 */
int main() {
    /* The input and expected/actual outputs of the function. */
    char *actual;
    int input_length, expected_length, actual_length;
    int i;

    i = 0;
    while (tests[i].decoded_str != NULL) {
        printf("Test case %d:  ", i);

        input_length = strlen(tests[i].encoded_str);
        expected_length = strlen(tests[i].decoded_str);

        actual_length = -100;
        actual = rl_decode(tests[i].encoded_str, input_length, &actual_length);

        if (expected_length != actual_length) {
            printf("\tFAIL:  actual size %d doesn't match expected size %d\n",
                   actual_length, expected_length);

            printf("\nExpected output:  ");
            print_buf(tests[i].decoded_str, expected_length);
            printf("\nActual output:    ");
            print_buf(actual, actual_length);
            printf("\n\n");
        }
        else if (memcmp(tests[i].decoded_str, actual, expected_length) != 0) {
            /* Must use memcmp(), not strcmp(), since the generated output is
             * not zero-terminated.
             */
            printf("\tFAIL:  sizes match, but actual result data doesn't "
                   "match expected result data\n");

            printf("\nExpected output:  ");
            print_buf(tests[i].decoded_str, expected_length);
            printf("\nActual output:    ");
            print_buf(actual, actual_length);
            printf("\n\n");
        }
        else {
            printf("\tPASS\n\n");
        }

        /* This was allocated on the heap... */
        free(actual);

        i++;
    }

    return 0;
}

