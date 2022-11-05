#ifndef BINTREE_H_INCLUDED__
#define BINTREE_H_INCLUDED__


#include <stdbool.h>
#include <stdint.h>
#include "bintree_settings.h"


typedef struct treeNode_t_
{
    struct treeNode_t_ *left;
    struct treeNode_t_ *right;
    treeData_t data;
} treeNode_t;


typedef struct
{
    size_t size;
    treeNode_t *root;
} tree_t;


enum TREE_CODES
{
    TREE_SUCCESS   = 0,
    TREE_ERROR     = 1,
    TREE_NULLPTR   = 2,
    TREE_NOMEM     = 3,
    TREE_EXISTS    = 4,
    TREE_NOTEXISTS = 5
};


enum TREE_CODES treeCtor(tree_t *tree);

bool treeEmpty(const tree_t *tree);

enum TREE_CODES treeInsertRoot(tree_t *tree, const treeData_t elem);

enum TREE_CODES treeInsertLeft(tree_t *tree, treeNode_t *node, const treeData_t elem);

enum TREE_CODES treeInsertRight(tree_t *tree, treeNode_t *node, const treeData_t elem);

enum TREE_CODES treeVerify(const tree_t *tree);

enum TREE_CODES treeDtor(tree_t *tree);


#endif /* BINTREE_H_INCLUDED__ */

