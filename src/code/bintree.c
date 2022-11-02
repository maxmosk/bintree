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

enum TREE_CODES treeInsert(tree_t *tree, treeData_t elem)
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
        treeNode_t *nextnode = tree->root;

        printf("root");
        do
        {
            node = nextnode;
            
            printf(" -- %s", node->data);
            int cmp = datacmp(elem, node->data);
            if (0 == cmp)
            {
                printf(";\n");
                return TREE_SUCCESS;
            }
            else if (0 < cmp)
            {
                if (NULL == node->left)
                {
                    node->left = calloc(1, sizeof *node->left);
                    nextnode = node->left;
                    node = NULL;
                }
                else
                {
                    nextnode = node->left;
                }
            }
            else if (0 > cmp)
            {
                if (NULL == node->right)
                {
                    node->right = calloc(1, sizeof *node->right);
                    nextnode = node->right;
                    node = NULL;
                }
                else
                {
                    nextnode = node->right;
                }
            }
        }
        while (NULL != node);
        printf(";\n");

        CHECK(NULL != nextnode, TREE_NOMEM);
        nextnode->data = elem;
        nextnode->left = nextnode->right = NULL;
    }

    return TREE_SUCCESS;
}

