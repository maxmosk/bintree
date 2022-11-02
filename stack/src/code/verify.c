#include "stack.h"



#define LOGIFERROR(COND, MSG)                                                   \
do {                                                                            \
    if (COND)                                                                   \
    {                                                                           \
        LOGPRINTF("%s", MSG);                                                   \
        LOGCLOSE();                                                             \
        return STACK_ERROR;                                                     \
    }                                                                           \
}                                                                               \
while (0)



/*(---------------------------------------------------------------------------*/
#ifndef NDEBUG_CANARY
static bool stackCheckCanary(const stack_t *stk)
{
    return (*((canary_t *) stk->data - 1) == CANARY_REFERENCE) &&
            (*((canary_t *) (stk->data + stk->capacity)) == CANARY_REFERENCE);
}
#endif /* NDEBUG_CANARY */
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
#ifndef NDEBUG_LOG
enum STACK_CODES stackVerify(stack_t *stk, const char *file,
                                        const char *func, int line)
#else
enum STACK_CODES stackVerify(stack_t *stk)
#endif /* NDEBUG_LOG */
{
    LOGOPEN("log.txt");

    LOGPRINTF("Verificator:  file %32s, function %32s, line %6d\n",
                                                file, func, line);

    LOGPRINTF("stack_t [%p]", (const void *) stk);
    LOGIFERROR(NULL == stk, "\n\t*** Error: invalid pointer ***\n");

    LOGPRINTF(" aka %s from file %s function %s line %d\n",
            stk->info.name, stk->info.file, stk->info.func, stk->info.line);
    LOGPRINTF("{\n");
#ifndef NDEBUG_CANARY
    LOGPRINTF("    leftCanary      = %lX\n", stk->leftCanary);
    LOGPRINTF("    rightCanary     = %lX\n", stk->rightCanary);
#endif /* NDEBUG_CANARY */
    LOGPRINTF("    capacity        = %zu\n", stk->capacity);
    LOGPRINTF("    size            = %zu\n", stk->size);
#ifndef NDEBUG_HASH
    hash_t calcHeadHash = stackHeadHash(stk);
    LOGPRINTF("    headHash        = %x (calculated: %x)\n", stk->headHash, calcHeadHash);
    hash_t calcDataHash = stackDataHash(stk);
    LOGPRINTF("    dataHash        = %x (calculated: %x)\n", stk->dataHash, calcDataHash);
#endif /* NDEBUG_HASH */
    LOGPRINTF("    status          = %lx\n", stk->status);
    LOGPRINTF("    {\n");
    LOGPRINTF("        Destructed %32s\n", (stk->status IS_CODE DESTRUCTED) ? "YES" : "NO");
    LOGPRINTF("        Underflow  %32s\n", (stk->status IS_CODE  UNDERFLOW) ? "YES" : "NO");
    LOGPRINTF("        Overflow   %32s\n", (stk->status IS_CODE   OVERFLOW) ? "YES" : "NO");
    LOGPRINTF("    }\n");
    LOGPRINTF("    data            = %p\n", (const void *) stk->data);

#ifndef NDEBUG_CANARY
    LOGIFERROR(CANARY_REFERENCE != stk->leftCanary, "\t*** Error: left canary is dead ***\n}\n");
    LOGIFERROR(CANARY_REFERENCE != stk->rightCanary, "\t*** Error: right canary is dead ***\n}\n");
#endif /* NDEBUG_CANARY */
#ifndef NDEBUG_HASH
    LOGIFERROR(calcHeadHash != stk->headHash, "\t*** Error: head hash is wrong ***\n}\n");
    LOGIFERROR(calcDataHash != stk->dataHash, "\t*** Error: data hash is wrong ***\n}\n");
#endif /* NDEBUG_HASH */
    LOGIFERROR(stk->size > stk->capacity, "\t*** Error: size is bigger then capacity ***\n}\n");
    LOGIFERROR(SIZE_POISON == stk->size, "\t*** Error: size is poison ***\n}\n");
    LOGIFERROR(SIZE_POISON == stk->capacity, "\t*** Error: capacity is poison ***\n}\n");
    LOGIFERROR(0 != stk->status, "\t*** Error: invalid stack status ***\n}\n");
    LOGIFERROR(((NULL == stk->data) && (0 != stk->capacity)) || (POINTER_POISON == stk->data), 
                                        "\t*** Error: invalid data pointer ***\n}\n");

#ifndef NDEBUG_CANARY
    LOGPRINTF("    leftDataCanary  = %lX\n", *((canary_t *) stk->data - 1));
    LOGPRINTF("    rightDataCanary = %lX\n", *((canary_t *) (stk->data + stk->capacity)));
    LOGIFERROR(!stackCheckCanary(stk), "\t*** Error: invalid data canary ***\n}\n");
#endif /* NDEBUG_CANARY */

    LOGPRINTF("    {\n");
    for (size_t i = 0; i < stk->capacity; i++)
    {
        LOGPRINTF("      %c [%9zu ] = ", (i < stk->size) ? '*' : ' ', i);
        LOGPRINTELEM(stk->data[i]);
        
        if (IS_ELEM_POISON(stk->data[i]))
        {
            LOGPRINTF(" (POISON)");
        }

        LOGPRINTF("\n");
    }
    LOGPRINTF("    }\n");
    LOGPRINTF("}\n");
    LOGCLOSE();


    return STACK_SUCCESS;
}
/*)---------------------------------------------------------------------------*/
