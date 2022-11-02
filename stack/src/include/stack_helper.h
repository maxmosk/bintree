#ifndef __STACK_HELPER_H_INCLUDED__
#define __STACK_HELPER_H_INCLUDED__



/*#define NDEBUG_CANARY*/
/*#define NDEBUG_HASH*/
/*#define NDEBUG_LOG*/
/*#define NDEBUG*/
#include <assert.h>
#include <stdio.h>
#include <debug.h>



#ifndef NDEBUG

#ifndef COLORFULL
#define ASSERTED == STACK_SUCCESS || (fprintf(stderr, "\t*** Error in file %s line %d ***\n", __FILE__, __LINE__), (exit(0), 1))
#else

#include "ansi_colors.h"

#define ASSERTED == STACK_SUCCESS || (fprintf(stderr, ANSI_COLOR_RED "\t*** Error in file %s line %d ***\n" ANSI_COLOR_RESET, __FILE__, __LINE__), (exit(0), 1))

#endif /* COLORFULL */

#else
#define ASSERTED
#endif /* NDEBUG */


#ifndef NDEBUG
#ifndef NDEBUG_LOG
#define STACK_OK(STK)                                                           \
do                                                                              \
{                                                                               \
    if (STACK_SUCCESS != stackVerify(STK, __FILE__, __func__, __LINE__))        \
    {                                                                           \
        assert(0);                                                              \
    }                                                                           \
}                                                                               \
while (0)

#else
#define STACK_OK(STK)                                                           \
do                                                                              \
{                                                                               \
    if (STACK_SUCCESS != stackVerify(STK))                                      \
    {                                                                           \
        STACK_ERROR ASSERTED;                                                   \
    }                                                                           \
}                                                                               \
while (0)
#endif /* NDEBUG_LOG */

#else
#define STACK_OK(STK)                                                           \
do                                                                              \
{                                                                               \
    if (STACK_SUCCESS != stackVerify(STK))                                      \
    {                                                                           \
        return STACK_ERROR;                                                     \
    }                                                                           \
}                                                                               \
while (0)

#endif /* NDEBUG */


#define POINTER_OK(PTR, RET)                                                    \
do                                                                              \
{                                                                               \
    assert(NULL != PTR);                                                        \
    if (NULL == PTR)                                                            \
    {                                                                           \
        return RET;                                                             \
    }                                                                           \
}                                                                               \
while (0)



#ifndef NDEBUG_LOG
#define LOGPRINTELEM(ELEM) PRINTELEM(log_opened, ELEM)

typedef struct
{
    int line;
    const char *name;
    const char *file;
    const char *func;
} stack_info_t;

#else
#define LOGPRINTELEM(ELEM) do {} while (0)
#endif /* NDEBUG_LOG */



#endif /* __STACK_HELPER_H_INCLUDED__ */

