#include "bintree.h"
#include "debug.h"


const size_t stackInitSize = 10;

static const char gvizbuf[] = "gvizbuffer";


static void treeGraph(const tree_t *tree, const char *filename);

static void treeGraphAddNode(const treeNode_t *node, FILE *file);


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
    enum TREE_CODES verify = TREE_ERROR;
    CHECK(TREE_SUCCESS == (verify = treeVerify(tree)), false);

    return NULL == tree->root;
}

enum TREE_CODES treeInsert(tree_t *tree, const enum CHILD_CODE child, const treeData_t elem)
{
    enum TREE_CODES verify = TREE_ERROR;
    CHECK(TREE_SUCCESS == (verify = treeVerify(tree)), verify);

    if (treeEmpty(tree) && (DUMMY_CHILD == child))
    {
        tree->root = calloc(1, sizeof *tree->root);
        CHECK(NULL != tree->root, TREE_NOMEM);

        tree->root->data = elem;
        tree->root->left = tree->root->right = NULL;

        CHECK(STACK_SUCCESS == stackPush(&tree->stack, (void *) tree->root), TREE_STACKERR);
        tree->level++;
    }
    else if ((LEFT_CHILD == child) || (RIGHT_CHILD == child))
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
        printf("%p %s\n", (void *) node, elem);

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
    else
    {
        return TREE_ERROR;
    }

    return TREE_SUCCESS;
}

enum TREE_CODES treeNext(tree_t *tree, const enum CHILD_CODE child)
{
    enum TREE_CODES verify = TREE_ERROR;
    CHECK(TREE_SUCCESS == (verify = treeVerify(tree)), verify);

    CHECK(!treeEmpty(tree), TREE_ERROR);
    CHECK(0 != tree->level, TREE_ERROR);

    treeNode_t *node = NULL;
    CHECK(STACK_SUCCESS == stackPop(&tree->stack, (void *) &node), TREE_STACKERR);
    CHECK(STACK_SUCCESS == stackPush(&tree->stack, (void *) node), TREE_STACKERR);

    if (LEFT_CHILD == child)
    {
        CHECK(NULL != node->left, TREE_NOTEXISTS);
        CHECK(STACK_SUCCESS == stackPush(&tree->stack, (void *) node->left), TREE_STACKERR);
    }
    else if (RIGHT_CHILD == child)
    {
        CHECK(NULL != node->right, TREE_NOTEXISTS);
        CHECK(STACK_SUCCESS == stackPush(&tree->stack, (void *) node->right), TREE_STACKERR);
    }

    tree->level++;

    return TREE_SUCCESS;
}

enum TREE_CODES treePrev(tree_t *tree)
{
    enum TREE_CODES verify = TREE_ERROR;
    CHECK(TREE_SUCCESS == (verify = treeVerify(tree)), verify);

    CHECK(!treeEmpty(tree), TREE_ERROR);
    CHECK(1 != tree->level, TREE_ERROR);

    treeNode_t *node = NULL;
    CHECK(STACK_SUCCESS == stackPop(&tree->stack, (void *) &node), TREE_STACKERR);
    tree->level--;

    return TREE_SUCCESS;
}

enum TREE_CODES treeVerify(tree_t *tree)
{
    CHECK(NULL != tree, TREE_NULLPTR);

    static size_t ngraphs = 0;
    char namebuf[64] = "";
    sprintf(namebuf, "graph%zu.jpg", ngraphs++);

    LOGOPEN("tree.html");
    LOGPRINTF("<pre>\n");
    LOGPRINTF("tree_t [%p]\n", (void *) tree);
    LOGPRINTF("{\n");
    LOGPRINTF("    root = %p\n", (void *) tree->root);
    LOGPRINTF("    level = %zu\n", tree->level);
    LOGPRINTF("}\n");

    treeGraph(tree, namebuf);
    LOGPRINTF("<img src=\"%s\">\n", namebuf);
    LOGPRINTF("</pre>\n");
    LOGCLOSE();

    return TREE_SUCCESS;
}

static void treeGraph(const tree_t *tree, const char *filename)
{
    CHECK(NULL != tree, ;);
    CHECK(NULL != filename, ;);

    FILE *dotfile = fopen(gvizbuf, "w");
    CHECK(NULL != dotfile, ;);

    fprintf(dotfile, "digraph\n");
    fprintf(dotfile, "{\n");

    if (NULL != tree->root)
    {
        fprintf(dotfile, "root->NODE%p\n", (const void *) tree->root);
        treeGraphAddNode(tree->root, dotfile);
    }

    fprintf(dotfile, "}\n");
    fclose(dotfile);

    char cmdbuf[128] = "";
    sprintf(cmdbuf, "dot %s -o %s -Tjpg", gvizbuf, filename);
    CHECK(0 == system(cmdbuf), ;);
}

static void treeGraphAddNode(const treeNode_t *node, FILE *file)
{
    if (NULL == node)
    {
        return;
    }
    else
    {
        fprintf(file, "NODE%p[style=\"rounded\",shape=record,color=\"blue\",label="
                        "\" <left> left=%p | data=%s | <right> right=%p\"];\n",
                        (const void *) node, (const void *) node->left, node->data,
                        (const void *) node->right);

        if (NULL != node->left)
        {
            fprintf(file, "NODE%p:left->NODE%p;\n", (const void *) node,
                                                (const void *) node->left);
        }

        if (NULL != node->right)
        {
            fprintf(file, "NODE%p:right->NODE%p;\n", (void *) node, (void *) node->right);
        }
    }

    treeGraphAddNode(node->left, file);
    treeGraphAddNode(node->right, file);
}

