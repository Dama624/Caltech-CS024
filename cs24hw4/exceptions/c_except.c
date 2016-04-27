#include "c_except.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "ptr_vector.h"




typedef struct exception_name {
    int exc_type;
    const char *exc_name;
} exception_name;


exception_name names[] = {
   { NO_EXCEPTION, "NO_EXCEPTION" },
   { UNSPECIFIED_EXCEPTION, "UNSPECIFIED_EXCEPTION" },
   { DIVIDE_BY_ZERO, "DIVIDE_BY_ZERO" },
   { NUMBER_PARSE_ERROR, "NUMBER_PARSE_ERROR" },
   { -1, "<unrecognized>" }
};


const char *get_exception_name(ExceptionType exc_type) {
    int i = 0;
    while (names[i].exc_type != -1 && names[i].exc_type != exc_type) i++;
    return names[i].exc_name;
}




static PtrStack env_stack = PTR_VECTOR_STATIC_INIT;




int start_try(jmp_buf *p_buf) {
    assert(p_buf != NULL);
    return ps_push_elem(&env_stack, p_buf);
}


void finish_try() {
    ps_pop_elem(&env_stack);
}


void throw_exception(ExceptionType exc_type) {
    jmp_buf *p_buf;
    
    if (env_stack.size == 0) {
        printf("Unhandled exception %s (%d), aborting!!!\n",
               get_exception_name(exc_type), exc_type);
        abort();
    }

    p_buf = (jmp_buf *) ps_pop_elem(&env_stack);

    longjmp(*p_buf, exc_type);
}


