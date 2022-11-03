#include "bintree.h"
#include "debug.h"


const size_t stackInitSize = 10;


enum TREE_CODES treeCtor(tree_t *tree)
{
    CHECK(NULL != tree, TREE_NULLPTR);

    tree->root = NULL;
    tree->status = 0;
    tree->level = 0;

    CHECK(STACK_SUCCESS == stackCtor(&tree->stack, stackInitSize), TREE_STACKERR);

    return TREE_SUCCESS;
}

bool treeEmpty(const tree_t *tree)
{
    CHECK(NULL != tree, true);

    return NULL == tree->root;
}

enum TREE_CODES treeInsert(tree_t *tree, enum CHILD_CODE child, treeData_t elem)
{
    CHECK(NULL != tree, TREE_NULLPTR);

    if (treeEmpty(tree))
    {
        tree->root = calloc(1, sizeof *tree->root);
        CHECK(NULL != tree->root, TREE_NOMEM);

        tree->root->data = elem;
        tree->root->left = tree->root->right = NULL;
    }
    else
    {
        treeNode_t *node = NULL;
        CHECK(STACK_SUCCESS == stackPop(&tree->stack, (void *) &node), TREE_STACKERR);
        CHECK(STACK_SUCCESS == stackPush(&tree->stack, (void *) node), TREE_STACKERR);

        if (LEFT_CHILD == child)
        {
            CHECK(NULL == node->left, TREE_EXISTS);
        }
        else if (RIGHT_CHILD == child)
        {
            CHECK(NULL == node->right, TREE_EXISTS);
        }

        treeNode_t *newnode = calloc(1, sizeof *newnode);
        CHECK(NULL != newnode, TREE_NOMEM);
        newnode->data = elem;
        newnode->right = newnode->left = NULL;
        
        if (LEFT_CHILD == child)
        {
            node->left = newnode;
        }
        else if (RIGHT_CHILD == child)
        {
            node->right = newnode;
        }
    }

    return TREE_SUCCESS;
}

