The files f1.txt through f4.txt are lists of floating-point values to sum
using the fsum summation program.  Each one contains 100,000 values to sum,
but different files have different characteristics:

  f1.txt, f2.txt - contain values in the range 10^-6 to 10^3
  f3.txt - contains values in the range 10^-8 to 10^-5
  f4.txt - contains values in the range 10^-8 to 10^6

You will notice that the different ranges of values cause the summation
operation to exhibit different levels of variation.

Note that each one of these files also includes the "accurate sum" of the
input values as the last line of the file.  You can see this by running:

  tail f1.txt

(The tail utility prints the last lines of a file.)

