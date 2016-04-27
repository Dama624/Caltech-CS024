#include "alloc.h"
#include "ptr_vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


/*! Change to #define to output garbage-collector statistics. */
#define GC_STATS

/*!
 * Change to #undef to cause the garbage collector to only run when it has to.
 * This dramatically improves performance.
 *
 * However, while testing GC, it's easiest if you try it all the time, so that
 * the number of objects being manipulated is small and easy to understand.
 */
#define ALWAYS_GC


/* Change to #define for other verbose output. */
#undef VERBOSE


void free_value(Value *v);
void free_lambda(Lambda *f);
void free_environment(Environment *env);

/*========================================================*
 * TODO:  Declarations of your functions might go here... *
 *========================================================*/
void mark_value(Value *v);
void mark_lambda(Lambda *f);
void mark_environment(Environment *env);
void mark_eval_stack(EvaluationContext *eval);

void sweep_values();
void sweep_lambdas();
void sweep_environments();

/*!
 * A growable vector of pointers to all Value structs that are currently
 * allocated.
 */
static PtrVector allocated_values;


/*!
 * A growable vector of pointers to all Lambda structs that are currently
 * allocated.  Note that each Lambda struct will only have ONE Value struct that
 * points to it.
 */
static PtrVector allocated_lambdas;


/*!
 * A growable vector of pointers to all Environment structs that are currently
 * allocated.
 */
static PtrVector allocated_environments;


#ifndef ALWAYS_GC

/*! Starts at 1MB, and is doubled every time we can't stay within it. */
static long max_allocation_size = 1048576;

#endif


void init_alloc() {
    pv_init(&allocated_values);
    pv_init(&allocated_lambdas);
    pv_init(&allocated_environments);
}


/*!
 * This helper function prints some helpful details about the current allocation
 * status of the program.
 */
void print_alloc_stats(FILE *f) {
    /*
    fprintf(f, "Allocation statistics:\n");
    fprintf(f, "\tAllocated environments:  %u\n", allocated_environments.size);
    fprintf(f, "\tAllocated lambdas:  %u\n", allocated_lambdas.size);
    fprintf(f, "\tAllocated values:  %u\n", allocated_values.size);
    */

    fprintf(f, "%d vals \t%d lambdas \t%d envs\n", allocated_values.size,
        allocated_lambdas.size, allocated_environments.size);
}


/*!
 * This helper function returns the amount of memory currently being used by
 * garbage-collected objects.  It is NOT the total amount of memory being used
 * by the interpreter!
 */ 
long allocation_size() {
    long size = 0;
    
    size += sizeof(Value) * allocated_values.size;
    size += sizeof(Lambda) * allocated_lambdas.size;
    size += sizeof(Value) * allocated_environments.size;
    
    return size;
}


/*!
 * This function heap-allocates a new Value struct, initializes it to be empty,
 * and then records the struct's pointer in the allocated_values vector.
 */
Value * alloc_value(void) {
    Value *v = malloc(sizeof(Value));
    memset(v, 0, sizeof(Value));

    pv_add_elem(&allocated_values, v);

    return v;
}


/*!
 * This function frees a heap-allocated Value struct.  Since a Value struct can
 * represent several different kinds of values, the function looks at the
 * value's type tag to determine if additional memory needs to be freed for the
 * value.
 *
 * Note:  It is assumed that the value's pointer has already been removed from
 *        the allocated_values vector!  If this is not the case, serious errors
 *        will almost certainly occur.
 */
void free_value(Value *v) {
    assert(v != NULL);

    /*
     * If value refers to a lambda, we don't free it here!  Lambdas are freed
     * by the free_lambda() function, and that is called when cleaning up
     * unreachable objects.
     */

    if (v->type == T_String || v->type == T_Atom || v->type == T_Error)
        free(v->string_val);

    free(v);
}



/*!
 * This function heap-allocates a new Lambda struct, initializes it to be empty,
 * and then records the struct's pointer in the allocated_lambdas vector.
 */
Lambda * alloc_lambda(void) {
    Lambda *f = malloc(sizeof(Lambda));
    memset(f, 0, sizeof(Lambda));

    pv_add_elem(&allocated_lambdas, f);

    return f;
}


/*!
 * This function frees a heap-allocated Lambda struct.
 *
 * Note:  It is assumed that the lambda's pointer has already been removed from
 *        the allocated_labmdas vector!  If this is not the case, serious errors
 *        will almost certainly occur.
 */
void free_lambda(Lambda *f) {
    assert(f != NULL);

    /* Lambdas typically reference lists of Value objects for the argument-spec
     * and the body, but we don't need to free these here because they are
     * managed separately.
     */

    free(f);
}


/*!
 * This function heap-allocates a new Environment struct, initializes it to be
 * empty, and then records the struct's pointer in the allocated_environments
 * vector.
 */
Environment * alloc_environment(void) {
    Environment *env = malloc(sizeof(Environment));
    memset(env, 0, sizeof(Environment));

    pv_add_elem(&allocated_environments, env);

    return env;
}


/*!
 * This function frees a heap-allocated Environment struct.  The environment's
 * bindings are also freed since they are owned by the environment, but the
 * binding-values are not freed since they are externally managed.
 *
 * Note:  It is assumed that the environment's pointer has already been removed
 *        from the allocated_environments vector!  If this is not the case,
 *        serious errors will almost certainly occur.
 */
void free_environment(Environment *env) {
    int i;

    /* Free the bindings in the environment first. */
    for (i = 0; i < env->num_bindings; i++) {
        free(env->bindings[i].name);
        /* Don't free the value, since those are handled separately. */
    }
    free(env->bindings);

    /* Now free the environment object itself. */
    free(env);
}

/*
 * mark_value: Changes a Value's marked flag to 1. If the value contains a
 *             Lambda or a Cons pair, also marks those.
 * 
 * Input:
 *      Value *v: A pointer to a Value object
 *
 * Output:
 *      None. The function will modify the object directly.
 */
void mark_value(Value *v)
{
    if (v != NULL &&
        v->marked != 1)
    {
        v->marked = 1; // Mark the value
        if (v->type == T_Lambda)
        {
            // Value refers to Lambda object; mark that too
            mark_lambda(v->lambda_val);
        }
        else if (v->type == T_ConsPair)
        {
            // Value refers to a Cons pair; mark those
            mark_value(v->cons_val.p_car);
            mark_value(v->cons_val.p_cdr);
        }
    }
}

/*
 * mark_lambda: Changes a Lambda's marked flag to 1. Also marks its argument
 *              and body specifications when the Lambda is not native
 *              (i.e. native_impl != 1). Marks its parent environment.
 * 
 * Input:
 *      Lambda *f: A pointer to a Lambda object
 *
 * Output:
 *      None. The function will modify the object directly.
 */
void mark_lambda(Lambda *f)
{
    if (f != NULL &&
        f->marked != 1)
    {
        f->marked = 1; // Mark the lambda
        if (f->native_impl != 1)
        {
            // If lambda is not native, mark its arg and body
            mark_value(f->arg_spec);
            mark_value(f->body);
        }
        mark_environment(f->parent_env); // Mark its parent environment
    }
}

/*
 * mark_environment: Changes an Environment's marked flag to 1. Also marks all
 *                   values contained within it, then marks its parent
 *                   environment.
 * 
 * Input:
 *      Environment *env: A pointer to a Lambda object
 *
 * Output:
 *      None. The function will modify the object directly.
 */
void mark_environment(Environment *env)
{
    int i;
    if (env->marked != 1)
    {
        env->marked = 1; // mark the environment
        for (i = 0; i < env->num_bindings; i++)
        {
            // mark all values in environment
            mark_value(env->bindings[i].value);
        }
        // mark parent environments until NULL
        if (env->parent_env != NULL)
        {
            mark_environment(env->parent_env);
        }
    }   
}

/*
 * mark_eval_stack: Marks the evaluation stack. Also marks non-NULL
 *                  environment, expression, and child_eval_result.
 * 
 * Input:
 *      EvaluationContext *eval: A pointer to a EvaluationContext object
 *
 * Output:
 *      None. The function will modify the object directly.
 */
void mark_eval_stack(EvaluationContext *eval)
{
    int i;

    if (eval->current_env != NULL)
    {
        mark_environment(eval->current_env);
    }
    if (eval->expression != NULL)
    {
        mark_value(eval->expression);
    }
    if (eval->child_eval_result != NULL)
    {
        mark_value(eval->child_eval_result);
    }
    for (i = 0; i < eval->local_vals.size; i++)
    {
        Value **ppv = (Value **) pv_get_elem(&(eval->local_vals), i);
        if (*ppv != NULL)
        {
            mark_value(*ppv);
        }
    }
}

/*
 * sweep_values: Sweeps pointervector of allocated Values. If a Value is
 *               marked, unmarks it. If a Value is unmarked, deletes it and
 *               sets it to NULL. Removes all NULL entries at the end.
 * 
 * Input:
 *      None. Sweeps all allocated Values.
 *
 * Output:
 *      None. The function modifies the Values directly. 
 */
void sweep_values()
{
    Value *val;
    int i;

    // Loop through all allocated values
    for (i = 0; i < allocated_values.size; i++)
    {
        val = (Value *) pv_get_elem(&allocated_values, i);
        if (val->marked == 1)
        {
            // Unmark a Value
            val->marked = 0;
        }
        else
        {
            // Delete the Value and set it to NULL
            free_value(val);
            pv_set_elem(&allocated_values, i, NULL);
        }
    }
    // Remove all NULL entries
    pv_compact(&allocated_values);
}

/*
 * sweep_lambdas: Sweeps pointervector of allocated Lambdas. If a Lambda is
 *                marked, unmarks it. If a Lambda is unmarked, deletes it and
 *                sets it to NULL. Removes all NULL entries at the end.
 *                Does not affect Lambdas' Values (i.e. assumes that if a
 *                Lambda's Values are unused, the Lambda is unused)
 * 
 * Input:
 *      None. Sweeps all allocated Lambda.
 *
 * Output:
 *      None. The function modifies the Lambdas directly. 
 */
void sweep_lambdas()
{
    Lambda *f;
    int i;

    // Loop through all allocated lambdas
    for (i = 0; i < allocated_lambdas.size; i++)
    {
        f = (Lambda *) pv_get_elem(&allocated_lambdas, i);
        if (f->marked == 1)
        {
            // Unmark a Lambda
            f->marked = 0;
        }
        else
        {
            // Delete the Lambda and set it to NULL
            free_lambda(f);
            pv_set_elem(&allocated_lambdas, i, NULL);
        }
    }
    // Remove all NULL entries
    pv_compact(&allocated_lambdas);
}

/*
 * sweep_environments: Sweeps pointervector of allocated Environments. If an 
 *                     Environment is marked, unmarks it. If an Environment is
 *                     unmarked, deletes it and sets it to NULL. Removes all
 *                     NULL entries at the end.
 * 
 * Input:
 *      None. Sweeps all allocated Environments.
 *
 * Output:
 *      None. The function modifies the Environments directly. 
 */
void sweep_environments()
{
    Environment *env;
    int i;

    // Loop through all allocated values
    for (i = 0; i < allocated_environments.size; i++)
    {
        env = (Environment *) pv_get_elem(&allocated_environments, i);
        if (env->marked == 1)
        {
            // Unmark a Value
            env->marked = 0;
        }
        else
        {
            // Delete the Value and set it to NULL
            free_environment(env);
            pv_set_elem(&allocated_environments, i, NULL);
        }
    }
    // Remove all NULL entries
    pv_compact(&allocated_environments);
}

/*!
 * This function performs the garbage collection for the Scheme interpreter.
 * It also contains code to track how many objects were collected on each run,
 * and also it can optionally be set to do GC when the total memory used grows
 * beyond a certain limit.
 */
void collect_garbage() {
    Environment *global_env;
    PtrStack *eval_stack;
    int i;
    EvaluationContext *eval;

#ifdef GC_STATS
    int vals_before, procs_before, envs_before;
    int vals_after, procs_after, envs_after;

    vals_before = allocated_values.size;
    procs_before = allocated_lambdas.size;
    envs_before = allocated_environments.size;
#endif

#ifndef ALWAYS_GC
    /* Don't perform garbage collection if we still have room to grow. */
    if (allocation_size() < max_allocation_size)
        return;
#endif

    /*==========================================================*
     * TODO:  Implement mark-and-sweep garbage collection here! *
     *                                                          *
     * Mark all objects referenceable from either the global    *
     * environment, or from the evaluation stack.  Then sweep   *
     * through all allocated objects, freeing unmarked objects. *
     *                                                          *
     * Reminder to self: DECLARE THESE FUNCTIONS @ TOP          *
     *==========================================================*/



    global_env = get_global_environment();
    eval_stack = get_eval_stack();

    /* ... TODO ... */
    // Mark everything
    mark_environment(global_env);
    for (i = 0; i < (*eval_stack).size; i++)
    {
        eval = (EvaluationContext *) pv_get_elem(eval_stack, i);
        mark_eval_stack(eval);
    }
    // Sweep everything
    sweep_values();
    sweep_lambdas();
    sweep_environments();

#ifndef ALWAYS_GC
    /* If we are still above the maximum allocation size, increase it. */
    if (allocation_size() > max_allocation_size) {
        max_allocation_size *= 2;

        printf("Increasing maximum allocation size to %ld bytes.\n",
            max_allocation_size);
    }
#endif
    
#ifdef GC_STATS
    vals_after = allocated_values.size;
    procs_after = allocated_lambdas.size;
    envs_after = allocated_environments.size;

    printf("GC Results:\n");
    printf("\tBefore: \t%d vals \t%d lambdas \t%d envs\n",
            vals_before, procs_before, envs_before);
    printf("\tAfter:  \t%d vals \t%d lambdas \t%d envs\n",
            vals_after, procs_after, envs_after);
    printf("\tChange: \t%d vals \t%d lambdas \t%d envs\n",
            vals_after - vals_before, procs_after - procs_before,
            envs_after - envs_before);
#endif
}

