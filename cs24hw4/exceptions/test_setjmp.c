#include "my_setjmp.h"
#include <stdio.h>

static jmp_buf global_env;

/* 
 * longjmp0_returns_1: Tests if longjmp(env, 0) for buffer env returns 1
 *
 * Inputs:
 *		None
 *
 * Output:
 *		Outputs 0 if longjmp(env, 0) sets setjmp = 1
 *		Outputs 1 if it does not
 *		Should never output -1
 *		**Results in infinite loop if it sets setjmp = 0**
 */ 
int longjmp0_returns_1()
{
	jmp_buf env;
	int error = setjmp(env);
	if (error == 0)
	{
		longjmp(env, 0);
	}
	else if (error == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	return -1;
}

/* 
 * longjmpn_returns_n: Tests if longjmp(env, n) for buffer env returns n
 *
 * Inputs:
 *		int n: any positive integer n to test longjmp(env, n)
 *
 * Output:
 *		Outputs 0 if longjmp(env, n) sets setjmp = n
 *		Outputs 1 if it does not
 *		Should never output -1
 *		**Results in infinite loop if it sets setjmp = 0**
 */ 
int longjmpn_returns_n(int n)
{
	jmp_buf env;
	int error = setjmp(env);
	if (error == 0)
	{
		longjmp(env, n);
	}
	else if (error == n)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	return -1;
}

/* 
 * longjmp_beyond_func1: Tests if longjmp correctly jumps across multiple 
 *						 function invocations. longjmp is called in the
 *						 companion function, longjmp_beyond_func2. A global
 *						 buffer, global_env, is used for this test.
 *
 * Inputs:
 *		int x: any integer to pass to longjmp_beyond_func2. x < 5 should
 *			   activate the longjmp.
 *
 * Output:
 *		Outputs 0 if longjmp can jump from longjmp_beyond_func2
 *		Should never output -1
 *		Should never output 1 if x < 5, but...
 */ 
int longjmp_beyond_func2(int x);
int longjmp_beyond_func1(int x)
{
	if (setjmp(global_env) == 0)
	{
		return longjmp_beyond_func2(x);
	}
	else
	{
		return 0;
	}
}
int longjmp_beyond_func2(int x)
{
	if (x < 5)
	{
		longjmp(global_env, 1);
	}
	else
	{
		return 1;
	}
	return -1;
}

/* 
 * longjmp_stack: Tests if setjmp and longjmp do not corrupt the stack.
 *
 * Inputs:
 *		None.
 *
 * Output:
 *		Outputs 0 if local variables remain unchanged after setjmp-longjmp.
 *		Outputs 1 if any local variable has changed.
 *		Should never output -1
 */ 
int longjmp_stack()
{
	int x1;
	int y1;
	int z1;
	int x2;
	int y2;
	int z2;
	x1 = 5;
	y1 = 16;
	z1 = 420;
	jmp_buf env;
	x2 = 69;
	y2 = 233;
	z2 = 119;
	if (setjmp(env) == 0)
	{
		longjmp(env, 1);
	}
	else
	{
		if (x1 == 5 &&
			y1 == 16 &&
			z1 == 420 &&
			x2 == 69 &&
			y2 == 233 &&
			z2 == 119)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return -1;
}

int main()
{
	// longjmp0_returns_1
	printf("longjmp(env, 0) returns 1:		    	 ");
	if (longjmp0_returns_1() == 0)
	{
		printf("PASS\n");
	}
	else
	{
		printf("FAIL\n");
	}

	// longjmpn_returns_n for n = 5
	printf("longjmp(env, 5) returns 5:		    	 ");
	if (longjmpn_returns_n(5) == 0)
	{
		printf("PASS\n");
	}
	else
	{
		printf("FAIL\n");
	}

	// longjmpn_returns_n for n = -99
	printf("longjmp(env, -99) returns -99:		         ");
	if (longjmpn_returns_n(-99) == 0)
	{
		printf("PASS\n");
	}
	else
	{
		printf("FAIL\n");
	}

	// longjmp_beyond_func1/2
	printf("longjmp works across multiple functions:         ");
	if (longjmp_beyond_func1(1) == 0)
	{
		printf("PASS\n");
	}
	else
	{
		printf("FAIL\n");
	}

	// longjmp_stack
	printf("setmp-longjmp doesn't corrupt stack:             ");
	if (longjmp_stack() == 0)
	{
		printf("PASS\n");
	}
	else
	{
		printf("FAIL\n");
	}
	return 0;
}
