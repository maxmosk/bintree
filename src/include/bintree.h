#ifndef BINTREE_H_INCLUDED__
#define BINTREE_H_INCLUDED__


#include "bintree_settings.h"
#include "stack.h"


typedef struct treeNode_t_
{
    struct treeNode_t_ *left;
    struct treeNode_t_ *right;
    treeData_t data;
} treeNode_t;


#endif /* BINTREE_H_INCLUDED__ */

