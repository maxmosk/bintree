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

typedef int treeStatus_t;

typedef struct
{
    treeNode_t *root;
    treeStatus_t status;
} tree_t;


enum TREE_CODES
{
    TREE_SUCCESS = 0,
    TREE_ERROR   = 1,
    TREE_NULLPTR = 2
};


enum TREE_CODES treeCtor(tree_t *tree);


#endif /* BINTREE_H_INCLUDED__ */

