/* A simple program to convert a file containing machine code in binary,
 * into hexadecimal values suitable for loading into the processor
 * simulator.
 *
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>


/* Maximum length of a line in the input. */
#define BUF_SIZE 1000


int prepare(int line_no, char *input, char *clean, char *comment);
int convert(const char *bits);


int main() {
    char input[BUF_SIZE];
    char clean[BUF_SIZE];
    char comment[BUF_SIZE];
    int line_no = 0;
    int status = 0;
    int value;

    while (fgets(input, BUF_SIZE, stdin) != NULL) {
        line_no++;
        if (!prepare(line_no, input, clean, comment)) {
            /* prepare() function prints an error message if given bad input.
             * Report that there was bad input by setting status to nonzero.
             */
            status = 1;
        }
        else {
            /* Input had no errors.  Convert to a hexadecimal number and print
             * out the result.
             */
            value = convert(clean);
            if (value >= 0)          /* It's an actual value. */
                printf("%x    %s\n", value, comment);
            else                     /* Just print out any comment. */
                printf("%s\n", comment);
        }
    }

    return status;
}


/* This helper function takes a raw line of input, strips off any comments,
 * and also strips out any whitespace contained within the line.  This way
 * the input can actually use whitespace and comments to clarify the code.
 */
int prepare(int line_no, char *input, char *clean, char *comment) {
    int i, j;

    assert(line_no > 0);
    assert(input != NULL);
    assert(clean != NULL);
    assert(comment != NULL);

    /* Truncate whitespace at the end of the input line. */
    for (i = strlen(input) - 1; i >= 0; i--) {
        if (isspace(input[i]))
            input[i] = 0;
        else
            break;
    }

    /* If this line contains a comment, find the start of the comment and
     * end the string there.
     */
    comment[0] = 0;
    for (i = 0; input[i] != 0; i++) {
        if (input[i] == '#') {
            /* Copy the comment itself into the "comment" buffer, and then
             * truncate the string before the comment.
             */
            strcpy(comment, input + i);
            input[i] = 0;
            break;
        }
    }

    i = j = 0;
    while (input[i] != 0) {
        if (input[i] == '0' || input[i] == '1') {
            /* Input character is a 0 or a 1, so copy it across to the
             * cleaned version.
             */
            clean[j] = input[i];
            i++;
            j++;
        }
        else if (isspace(input[i])) {
            /* Skip over whitespace in the input. */
            i++;
        }
        else {
            /* This character is not a 1, 0, or a whitespace character.
             * Report the bad input to the caller.
             */
            printf("ERROR:  Line %d contains invalid characters:  %s\n",
                line_no, input);
            return 0;
        }
    }

    /* If we got here, we successfully converted the input.  Terminate the
     * clean string with a 0 value.  j also happens to be the length of the
     * input string.
     */

    clean[j] = 0;

    if (j > 16) {
        printf("ERROR:  Line %d has too many bits:  %s\n", line_no, input);
        return 0;
    }

    /* Return nonzero to indicate success. */
    return 1;
}


/* This function takes a string containing only '0' and '1' characters,
 * specifying a numeric value that must be converted from binary into an
 * actual integer value.  The most-significant bit (MSB) is contained in
 * bits[0], and the least-significant bit (LSB) is the last bit in the
 * string.  (Just like a normal decimal number, e.g. 653, where the most
 * significant digit is the leftmost, etc.)
 *
 * The function must convert this sequence of bits into a numeric value,
 * and then return the value.
 *
 * This function will never be given a sequence longer than 16 bits, so
 * we don't need to worry about signed/unsigned issues.
 */
int convert(const char *bits) {
    int length, value;

    assert(bits != 0);

    length = strlen(bits);
    if (length == 0) {
        /* No input, just skip. */
        return -1;
    }

    assert(length <= 16);

    /*=============================================================*/
    /* TODO:  Implement this part of the function!                 */
    /*                                                             */
    /*        You need to iterate over the '0' and '1' characters  */
    /*        in the input array, and use them to set the "value"  */
    /*        variable to the actual value represented by that     */
    /*        string of bits.                                      */
    /*=============================================================*/

    return value;
}

