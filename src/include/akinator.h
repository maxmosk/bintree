#ifndef AKINATOR_H_INCLUDED__
#define AKINATOR_H_INCLUDED__


#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "bintree.h"


enum MODES
{
    AKINATOR   = 0,
    DEFINITION = 1,
    DIFFERENCE = 2,
    MODE_ERROR = 3
};

enum CODES
{
    SUCCESS         = 0,
    WRONG_MODE      = 1,
    NULLPTR_ERROR   = 2,
    TREE_OPS_ERROR  = 3,
    DATABASE_ERROR  = 4,
    STACK_OPS_ERROR = 5,
    FILE_ERROR      = 6,
    NO_MEMORY_ERROR = 7
};


enum CODES play(void);


#endif /* AKINATOR_H_INCLUDED__ */
