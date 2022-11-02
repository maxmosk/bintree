#ifndef BINTREE_H_INCLUDED__
#define BINTREE_H_INCLUDED__


#include <stdbool.h>
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
    stack_t stack;
    size_t level;
    treeNode_t *root;
    treeStatus_t status;
} tree_t;


enum TREE_CODES
{
    TREE_SUCCESS  = 0,
    TREE_ERROR    = 1,
    TREE_NULLPTR  = 2,
    TREE_NOMEM    = 3,
    TREE_STACKERR = 4
};


enum TREE_CODES treeCtor(tree_t *tree);

bool treeEmpty(const tree_t *tree);

enum TREE_CODES treeInsert(tree_t *tree, treeData_t elem);


#endif /* BINTREE_H_INCLUDED__ */

