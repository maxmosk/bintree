#include "bintree.h"
#include "debug.h"


enum TREE_CODES treeCtor(tree_t *tree)
{
    CHECK(NULL != tree, TREE_NULLPTR);

    tree->root = NULL;
    tree->status = 0;

    return TREE_SUCCESS;
}

bool treeEmpty(const tree_t *tree)
{
    CHECK(NULL != tree, true);

    return NULL == tree->root;
}

enum TREE_CODES treeInsert(tree_t *tree, treeData_t elem)
{
    CHECK(NULL != tree, TREE_NULLPTR);

    if (treeEmpty(tree))
    {
        tree->root = calloc(1, sizeof *tree->root);
        CHECK(NULL != tree->root, TREE_NOMEM);

        tree->root->data = elem;
        tree->root->left = NULL;
        tree->root->right = NULL;
    }

    return TREE_SUCCESS;
}

