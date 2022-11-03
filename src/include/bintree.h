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


typedef struct
{
    stack_t stack;
    size_t level;
    treeNode_t *root;
} tree_t;


enum TREE_CODES
{
    TREE_SUCCESS   = 0,
    TREE_ERROR     = 1,
    TREE_NULLPTR   = 2,
    TREE_NOMEM     = 3,
    TREE_STACKERR  = 4,
    TREE_EXISTS    = 5,
    TREE_NOTEXISTS = 6
};

enum CHILD_CODE
{
    LEFT_CHILD  = 0,
    RIGHT_CHILD = 1,
    DUMMY_CHILD = 2
};


enum TREE_CODES treeCtor(tree_t *tree);

bool treeEmpty(const tree_t *tree);

enum TREE_CODES treeInsert(tree_t *tree, const enum CHILD_CODE child, const treeData_t elem);

enum TREE_CODES treeNext(tree_t *tree, const enum CHILD_CODE child);

enum TREE_CODES treePrev(tree_t *tree);

enum TREE_CODES treeVerify(const tree_t *tree);

treeData_t treeData(tree_t *tree);

enum TREE_CODES treeDtor(tree_t *tree);


#endif /* BINTREE_H_INCLUDED__ */

