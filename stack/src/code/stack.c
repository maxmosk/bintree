#include "stack.h"



/*(===========================================================================*/
static const size_t lowerSize = 8;


static const size_t higherSize = 8192;
/*)===========================================================================*/



/*(===========================================================================*/
#ifndef NDEBUG_HASH 
#define SET_HASH(stk)                                                           \
do                                                                              \
{                                                                               \
    stk->dataHash = stackDataHash(stk);                                         \
    stk->headHash = stackHeadHash(stk);                                         \
}                                                                               \
while (0)
#else
#define SET_HASH(stk) do { } while (0)
#endif /* NDEBUG_HASH */
/*)===========================================================================*/



/*(===========================================================================*/
static elem_t *stackAlloc(size_t size);

static elem_t *stackRealloc(stack_t *stk, size_t size);

static size_t stackIncreaseSize(const stack_t *stk);

static size_t stackCalcSize(size_t size);

static void stackFree(elem_t *data);

static enum STACK_CODES stackPoisonMemory(void *mem, size_t size);

#ifndef NDEBUG_CANARY
static enum STACK_CODES stackSetCanary(void *mem, size_t size);
#endif /* NDEBUG_CANARY */
/*)===========================================================================*/



/*(===========================================================================*/
/*(---------------------------------------------------------------------------*/
#ifndef NDEBUG_LOG
enum STACK_CODES
stackCtor_(stack_t *stk, size_t size, const char *file, const char *func,
        int line, const char *name)
#else
enum STACK_CODES stackCtor_(stack_t *stk, size_t size)
#endif /* NDEBUG_LOG */
{
#if defined(NDEBUG_HASH) && defined(NDEBUG_CANARY)
    STACK_OK(stk);
#endif /* NDEBUG_HASH and NDEBUG_CANARY */

    stk->size = 0;
    stk->capacity = size;

    stk->data = stackAlloc(size);
    if (NULL == stk->data)
    {
        return STACK_ERROR;
    }

#ifndef NDEBUG_LOG
    stk->info.file = file;
    stk->info.line = line;
    stk->info.func = func;
    stk->info.name = name + 1;
#endif /* NDEBUG_LOG */

#ifndef NDEBUG_CANARY
    stk->leftCanary  = CANARY_REFERENCE;
    stk->rightCanary = CANARY_REFERENCE;
#endif /* NDEBUF_CANARY */
    
    SET_HASH(stk);

    return STACK_SUCCESS;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
enum STACK_CODES stackPush(stack_t *stk, elem_t item)
{
    STACK_OK(stk);

    if (stk->size == stk->capacity)
    {
        size_t newStkSize = stackIncreaseSize(stk);
        elem_t *newData = stackRealloc(stk, newStkSize);
        if (NULL == newData)
        {
            stk->status SET_CODE OVERFLOW;
            return STACK_ERROR;
        }
        else
        {
            stk->data = newData;
            stk->capacity = newStkSize;
        }
    }

    stk->data[stk->size++] = item;

    SET_HASH(stk);

    return STACK_SUCCESS;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
enum STACK_CODES stackPop(stack_t *stk, elem_t *item)
{
    STACK_OK(stk);
    POINTER_OK(item, STACK_ERROR);

    if (0 == stk->size)
    {
        stk->status SET_CODE UNDERFLOW;
        return STACK_ERROR;
    }

    *item = stk->data[--stk->size];
    stk->data[stk->size] = ELEM_POISON;
    
    if ((stk->size * 3 < stk->capacity) && (stk->capacity > lowerSize * 2))
    {
        elem_t *newData = stackRealloc(stk, stk->capacity / 2);
        if (NULL != newData)
        {
            stk->data = newData;
            stk->capacity /= 2;
        }
    }

    SET_HASH(stk);

    return STACK_SUCCESS;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
static elem_t *stackAlloc(size_t size)
{
    elem_t *ptr = calloc(stackCalcSize(size), 1);

    LOGOPEN("log.txt");
    LOGPRINTF("Alloc pointer %p size %zu\n of elem_t", (void *) ptr, size);
    LOGCLOSE();

    if (NULL == ptr)
    {
        return ptr;
    }

#ifndef NDEBUG_CANARY
    ptr = (canary_t *) ptr + 1;
    stackSetCanary(ptr, size);
#endif /* NDEBUG_CANARY */
    stackPoisonMemory(ptr, size);

    return ptr;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
#ifndef NDEBUG_LOG
enum STACK_CODES
stackDtor_(stack_t *stk, const char *file, const char *func, int line)
#else
enum STACK_CODES stackDtor_(stack_t *stk)
#endif /* NDEBUG_LOG */
{
    STACK_OK(stk);

    stackFree(stk->data);
    stk->data = POINTER_POISON;

    stk->capacity = CAPACITY_POISON;
    stk->size = SIZE_POISON;
    stk->status SET_CODE DESTRUCTED;

    return STACK_SUCCESS;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
static elem_t *stackRealloc(stack_t *stk, size_t size)
{
#ifndef NDEBUG_CANARY
    void *oldData = (canary_t *) stk->data - 1;
#else
    void *oldData = stk->data;
#endif /* NDEBUG_CANARY */

    elem_t *newData = realloc(oldData, stackCalcSize(size));

    LOGOPEN("log.txt");
    LOGPRINTF("Realloc pointer %p size %zu of elem_t to pointer %p size %zu\n of elem_t",
            (void *) oldData, stk->capacity, (void *) newData, size);
    LOGCLOSE();

    if (NULL == newData)
    {
        return NULL;
    }
    else
    {
#ifndef NDEBUG_CANARY
        newData = (canary_t *) newData + 1;
        stackSetCanary(newData, size);
#endif /* NDEBUG_CANARY */
        stackPoisonMemory(newData + stk->size, size - stk->size);

        return newData;
    }
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
static size_t stackIncreaseSize(const stack_t *stk)
{
    if (stk->capacity < lowerSize)
    {
        return lowerSize;
    }
    else if (stk->capacity >= higherSize)
    {
        return stk->capacity + stk->capacity / 2;
    }
    else
    {
        return stk->capacity * 2;
    }
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
static size_t stackCalcSize(size_t size)
{
#ifndef NDEBUG_CANARY
    const size_t addSize = 2 * sizeof (canary_t);
#else
    const size_t addSize = 0;
#endif /* NDEBUG_CANARY */

    return size * sizeof (elem_t) + addSize;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
static void stackFree(elem_t *data)
{
    LOGOPEN("log.txt");
    LOGPRINTF("Free pointer %p\n", (void *) data);
    LOGCLOSE();

#ifndef NDEBUG_CANARY
    data = (canary_t *) data - 1;
#endif /* NDEBUG_CANARY */
    free(data);
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
static enum STACK_CODES stackPoisonMemory(void *mem, size_t size)
{
    POINTER_OK(mem, STACK_ERROR);

    elem_t *elemMem = (elem_t *) mem;
    for (size_t i = 0; i < size; i++)
    {
        elemMem[i] = ELEM_POISON;
    }

    return STACK_SUCCESS;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
#ifndef NDEBUG_CANARY
static enum STACK_CODES stackSetCanary(void *mem, size_t size)
{
    POINTER_OK(mem, STACK_ERROR);

    *((canary_t *) mem - 1) = CANARY_REFERENCE;
    *((canary_t *) ((elem_t *) mem + size)) = CANARY_REFERENCE;

    return STACK_SUCCESS;
}
#endif /* NDEBUG_CANARY */
/*)---------------------------------------------------------------------------*/


#ifndef NDEBUG_HASH
/*(---------------------------------------------------------------------------*/
hash_t stackHeadHash(stack_t *stk)
{
    POINTER_OK(stk, 0);

    hash_t tmpHeadHash = stk->headHash;
    hash_t tmpDataHash = stk->dataHash;

    stk->headHash = 0;
    stk->dataHash = 0;

    hash_t hash = hashFAQ6(stk, sizeof *stk);

    stk->headHash = tmpHeadHash;
    stk->dataHash = tmpDataHash;

    return hash;
}
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
hash_t stackDataHash(const stack_t *stk)
{
    POINTER_OK(stk, 0);
    POINTER_OK(stk->data, 0);
    
    return hashFAQ6(stk->data, stk->capacity * sizeof (elem_t));
}
/*)---------------------------------------------------------------------------*/
#endif /* NDEBUG_HASH */
/*)===========================================================================*/

