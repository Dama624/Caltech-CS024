#ifndef C_EXCEPT
#define C_EXCEPT

#include "my_setjmp.h"


typedef enum ExceptionType {
  /*---------------------------------------------------------------------------
   * These are special values.  DO NOT MODIFY!
   */
   
  /* This is what setjmp() returns when it isn't the result of a longjmp(). */
  NO_EXCEPTION = 0,
  
  /* This is what setjmp() returns when longjmp(0) was called. */
  UNSPECIFIED_EXCEPTION = 1, 

  /* All values in this enumeration should be larger than this value. */
  EXCEPTION_TYPES_START = 100,
  
  /*---------------------------------------------------------------------------
   * This is where the actual exception types start.
   */
   
  DIVIDE_BY_ZERO,
  
  NUMBER_PARSE_ERROR
} ExceptionType;


int start_try(jmp_buf *p_buf);
void finish_try();
void throw_exception(ExceptionType exc_type);


#define TRY(code) \
  { \
    jmp_buf env; \
    int exception = setjmp(env); \
    /* printf("exception = %d\n", exception); */ \
    if (exception == NO_EXCEPTION) { \
      start_try(&env); \
      { \
        code \
      } \
      finish_try();

#define CATCH(exc_type, code) \
    } \
    else if (exception == exc_type) { \
      code

#define END_TRY \
    } \
    else { \
      RETHROW; \
    } \
  }

#define THROW(exc_type) throw_exception(exc_type)

#define RETHROW throw_exception(exception)


#endif /* C_EXCEPT */

