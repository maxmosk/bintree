#ifndef __STACK_H_INCLUDED__
#define __STACK_H_INCLUDED__



#include "stack_settings.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "stack_helper.h"



/*(---------------------------------------------------------------------------*/
enum STACK_CODES
{
    STACK_SUCCESS = 0,
    STACK_ERROR = 1
};
/*)---------------------------------------------------------------------------*/



/*(---------------------------------------------------------------------------*/
/*
 *  Bit No.     Description
 *  0           Stack is destructed
 *  1           Underflow
 *  2           Overflow (memory error in overflow)
 */
typedef uint16_t stack_status_t;

enum STACK_STATUS_CODES
{
    DESTRUCTED      = 1 << 0,
    UNDERFLOW       = 1 << 1,
    OVERFLOW        = 1 << 2
};


#define IS_CODE &


#define SET_CODE |=
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
typedef uint64_t canary_t;

#define CANARY_REFERENCE ((canary_t) 0x919AE132F07DED32)


typedef struct
{
#ifndef NDEBUG_CANARY
    canary_t leftCanary;
#endif /* NDEBUG_CANARY */

    elem_t *data;
    size_t size;
    size_t capacity;
    stack_status_t status;

#ifndef NDEBUG_LOG
    stack_info_t info;
#endif /* NDEBUG_LOG */

#ifndef NDEBUG_HASH
    hash_t headHash;
    hash_t dataHash;
#endif /* NDEBUG_HASH */

#ifndef NDEBUG_CANARY
    canary_t rightCanary;
#endif /* NDEBUG_CANARY */
} stack_t;
/*)---------------------------------------------------------------------------*/




/*(---------------------------------------------------------------------------*/
#ifndef NDEBUG_LOG
#define stackCtor(stk, size) stackCtor_((stk), (size), __FILE__, __func__, __LINE__, #stk)
#else
#define stackCtor stackCtor_
#endif /* NDEBUG_LOG */
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
#ifndef NDEBUG_LOG
#define stackDtor(stk) stackDtor_((stk), __FILE__, __func__, __LINE__)
#else
#define stackDtor stackDtor_
#endif /* NDEBUG_LOG */
/*)---------------------------------------------------------------------------*/


/*(---------------------------------------------------------------------------*/
#define POINTER_POISON ((void *) 15)

#define SIZE_POISON SIZE_MAX

#define CAPACITY_POISON SIZE_MAX
/*)---------------------------------------------------------------------------*/



#ifndef NDEBUG_LOG
enum STACK_CODES
stackCtor_(stack_t *stk, size_t size, const char *file, const char *func,
        int line, const char *name);
#else
enum STACK_CODES stackCtor_(stack_t *stk, size_t size);
#endif /* NDEBUG_LOG */


enum STACK_CODES stackPush(stack_t *stk, elem_t item);


enum STACK_CODES stackPop(stack_t *stk, elem_t *item);


#ifndef NDEBUG_HASH
hash_t stackHeadHash(stack_t *stk);


hash_t stackDataHash(const stack_t *stk);
#endif /* NDEBUG_HASH */


#ifndef NDEBUG_LOG
enum STACK_CODES
stackDtor_(stack_t *stk, const char *file, const char *func, int line);
#else
enum STACK_CODES stackDtor_(stack_t *stk);
#endif /* NDEBUG_LOG */


#ifndef NDEBUG_LOG
enum STACK_CODES stackVerify(stack_t *stk, const char *file,
                                        const char *func, int line);
#else
enum STACK_CODES stackVerify(stack_t *stk);
#endif /* NDEBUG_LOG */



#endif /* __STACK_H_INCLUDED__ */

