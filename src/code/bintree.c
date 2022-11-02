#include "bintree.h"
#include "debug.h"


enum TREE_CODES treeCtor(tree_t *tree)
{
    CHECK(NULL != tree, TREE_NULLPTR);

    tree->root = NULL;
    tree->status = 0;

    return TREE_SUCCESS;
}

