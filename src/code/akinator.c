#include "akinator.h"
#include "debug.h"


/*(===========================================================================*/
const char *databasefile = "../data.txt";
/*)===========================================================================*/


/*(===========================================================================*/
static enum MODES getmode(void);

static enum CODES akinator(tree_t *data);

static enum CODES unknowen(tree_t *data, treeNode_t *node);

static char *readdatabase(const char *dbname, tree_t *dest);

static long long getfilesize(const char *filename);

static char *parsedata(tree_t *dest, treeNode_t *node, char *src);

static void savedata(FILE *file, treeNode_t *subtree);

static char *getresp(void);

static enum CODES definition(tree_t *data);

static bool definition_rec(const treeNode_t *node, const char *elem);
/*)===========================================================================*/


/*(===========================================================================*/
enum CODES play(void)
{
    tree_t database = {0};
    CHECK(TREE_SUCCESS == treeCtor(&database), TREE_OPS_ERROR);
    char *datatext = readdatabase(databasefile, &database);
    CHECK(NULL != datatext, DATABASE_ERROR);

    enum MODES mode = getmode();
    switch (mode)
    {
        case AKINATOR:
        {
            enum CODES status = akinator(&database);
            CHECK(SUCCESS == status, status);
        }
            break;
        case DEFINITION:
        {
            enum CODES status = definition(&database);
            CHECK(SUCCESS == status, status);
        }
            break;
        case DIFFERENCE:

            break;

        case MODE_ERROR:
        default:
            printf("Undefined mode\n");
            CHECK(TREE_SUCCESS == treeDtor(&database), TREE_OPS_ERROR);
            free(datatext);
            return WRONG_MODE;
    }

    CHECK(TREE_SUCCESS == treeDtor(&database), TREE_OPS_ERROR);
    free(datatext);

    return SUCCESS;
}
/*)===========================================================================*/


/*(===========================================================================*/
static enum MODES getmode(void)
{
    printf("Please, choose mode:\n");
    printf("(1) Akinator;\n");
    printf("(2) Definition;\n");
    printf("(3) Difference;\n");

    int resp = EOF;
    while (EOF != (resp = getchar()))
    {
        switch (resp)
        {
            case '1':
                return AKINATOR;
            case '2':
                return DEFINITION;
            case '3':
                return DIFFERENCE;

            default:
                return MODE_ERROR;
        }
    }

    return MODE_ERROR;
}

static enum CODES akinator(tree_t *data)
{
    CHECK(NULL != data, NULLPTR_ERROR);

    treeNode_t *node = data->root;
    while ((NULL != node->left) && (NULL != node->right))
    {
        printf("It is %s? ", node->data.string);

        int resp = '\0';
        do
        {
            resp = getchar();
        }
        while (('y' != resp) && ('n' != resp));

        if (resp == 'y')
        {
            node = node->left;
        }
        else if (resp == 'n')
        {
            node = node->right;
        }
    }

    printf("It is %s? ", node->data.string);
    int resp = '\0';
    do
    {
        resp = getchar();
    }
    while (('y' != resp) && ('n' != resp));

    if (resp == 'y')
    {
        printf("I said...\n");
    }
    else if (resp == 'n')
    {
        while (getchar() != '\n') { ; }
        enum CODES status = unknowen(data, node);
        CHECK(SUCCESS == status, status);
    }

    FILE *save = fopen(databasefile, "w");
    CHECK(NULL != save, FILE_ERROR);
    savedata(save, data->root);
    fclose(save);

    return SUCCESS;
}

static enum CODES unknowen(tree_t *data, treeNode_t *node)
{
    CHECK(NULL != data, NULLPTR_ERROR);

    printf("But what is it? ");

    char *newdata = getresp();
    CHECK(NULL != newdata, NO_MEMORY_ERROR);

    printf("What is the difference between %s and %s in all? ", newdata, node->data.string);

    char *olddata = node->data.string;
    node->data.string = getresp();
    CHECK(NULL != node->data.string, NO_MEMORY_ERROR);
    node->data.alloced = true;

    treeData_t leftelem = {newdata, true};
    CHECK(TREE_SUCCESS == treeInsertLeft(data, node, leftelem), TREE_OPS_ERROR);

    treeData_t rightelem = {olddata, false};
    CHECK(TREE_SUCCESS == treeInsertRight(data, node, rightelem), TREE_OPS_ERROR);

    return SUCCESS;
}

static char *readdatabase(const char *dbname, tree_t *dest)
{
    CHECK(NULL != dbname, NULL);
    CHECK(NULL != dest,   NULL);

    long long filesize = getfilesize(dbname);
    CHECK(filesize > 0, NULL);

    char *data = calloc((size_t) filesize + 1, sizeof *data);
    CHECK(NULL != data, NULL);

    FILE *datafile = fopen(dbname, "r");
    if (NULL == datafile)
    {
        free(data);
        return NULL;
    }

    if (filesize != (long long) fread(data, sizeof *data, (size_t) filesize, datafile))
    {
        free(data);
        fclose(datafile);
        return NULL;
    }

    fclose(datafile);

    treeData_t nulldata = {NULL};
    CHECK(TREE_SUCCESS == treeInsertRoot(dest, nulldata), NULL);
    if (NULL == parsedata(dest, dest->root, data))
    {
        free(data);
        return NULL;
    }

    return data;
}

static long long getfilesize(const char *filename)
{
    CHECK(NULL != filename, LONG_MIN);

    struct stat buf = {0};
    CHECK(0 == stat(filename, &buf), -1);

    return buf.st_size;
}

static char *parsedata(tree_t *dest, treeNode_t *node, char *src)
{
    CHECK(NULL != dest, NULL);
    CHECK(NULL != node, NULL);
    CHECK(NULL != src,  NULL);

    int shift = 0;
    char buf = '\0';
    sscanf(src, " %c %n", &buf, &shift);

    if ('{' == buf)
    {
        src += shift;
        node->data.string = strchr(src, '"') + 1;
        CHECK(NULL != node->data.string, NULL);

        char *nextsep = strchr(node->data.string, '"');
        CHECK(NULL != nextsep, NULL);
        *nextsep = '\0';
        src = nextsep + 1;

        treeData_t nulldata = {NULL, false};
        CHECK(TREE_SUCCESS == treeInsertLeft(dest, node, nulldata), NULL);
        src = parsedata(dest, node->left, src);
        CHECK(NULL != src, NULL);

        CHECK(TREE_SUCCESS == treeInsertRight(dest, node, nulldata), NULL);
        src = parsedata(dest, node->right, src);
        CHECK(NULL != src, NULL);

        char *closer = strchr(src, '}');
        CHECK(NULL != closer, NULL);
        *closer = '\0';
        src = closer + 1;
    }
    else if ('"' == buf)
    {
        node->data.string = src += shift;
        
        char *nextsep = strchr(node->data.string, '"');
        CHECK(NULL != nextsep, NULL);
        *nextsep = '\0';
        src = nextsep + 1;
    }
    else
    {
        return NULL;
    }

    return src;
}

static void savedata(FILE *file, treeNode_t *subtree)
{
    CHECK(NULL != file,    ;);
    CHECK(NULL != subtree, ;);

    if ((NULL == subtree->left) && (NULL == subtree->right))
    {
        fprintf(file, "\"%s\"\n", subtree->data.string);
    }
    else
    {
        fprintf(file, "{\n");
        fprintf(file, "\"%s\"\n", subtree->data.string);
        savedata(file, subtree->left);
        savedata(file, subtree->right);
        fprintf(file, "}\n");
    }
}

static char *getresp(void)
{
    char *data = NULL;
    size_t len = 64;
    CHECK(-1 != getline(&data, &len, stdin), NULL);

    char *newline = strchr(data, '\n');
    if (NULL != newline)
    {
        *newline = '\0';
    }

    return data;
}

static enum CODES definition(tree_t *data)
{
    CHECK(NULL != data, NULLPTR_ERROR);

    printf("Input some to define: ");

    while (getchar() != '\n') { ; }
    char *lookdata = getresp();
    CHECK(NULL != lookdata, NO_MEMORY_ERROR);

    if (!definition_rec(data->root, lookdata))
    {
        printf("%s is undefined!\n", lookdata);
    }
    else
    {
        putchar('\n');
    }

    return SUCCESS;
}

static bool definition_rec(const treeNode_t *node, const char *elem)
{
    CHECK(NULL != node, false);
    CHECK(NULL != elem, false);

    if ((NULL != node->left) && (NULL != node->right))
    {
        if (definition_rec(node->left, elem))
        {
            printf(", %s", node->data.string);
            return true;
        }
        else if (definition_rec(node->right, elem))
        {
            printf(", not %s", node->data.string);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        CHECK(NULL != node->data.string, false);
        if (0 == strcasecmp(elem, node->data.string))
        {
            printf("%s is defined", elem);
            return true;
        }
    }

    return false;
}
/*)===========================================================================*/
