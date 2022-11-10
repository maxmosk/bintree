#ifndef BINTREE_SETTINGS_H_INCLUDED__
#define BINTREE_SETTINGS_H_INCLUDED__


#include <string.h>
#include <stdbool.h>


#define DATA_POISON NULL


typedef struct
{
    char *string;
    bool alloced;
} treeData_t;


#endif /* BINTREE_SETTINGS_H_INCLUDED__ */

