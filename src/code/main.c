#include <stdio.h>
#include <string.h>
#include "bintree.h"
#include "debug.h"


enum MODES
{
    AKINATOR   = 0,
    DEFINITION = 1,
    DIFFERENCE = 2,
    MODE_ERROR = 3
};

enum CODES
{
    SUCCESS        = 0,
    WRONG_MODE     = 1,
    NULLPTRERR     = 2,
    TREE_OPS_ERROR = 3
};


enum MODES getMode(void);

enum CODES play(void);

enum CODES akinator(tree_t *data);

enum CODES unknowen(tree_t *data, enum CHILD_CODE node);

void datafree(char **data, void *dummy);


int main(void)
{
    play();

    return 0;
}


enum CODES play(void)
{
    tree_t data = {0};
    treeCtor(&data);

    enum MODES mode = getMode();
    switch (mode)
    {
        case AKINATOR:
            akinator(&data);
            break;
        case DEFINITION:

            break;
        case DIFFERENCE:

            break;

        case MODE_ERROR:
        default:
            printf("Undefined mode\n");
            treeDtor(&data);
            return WRONG_MODE;
    }

    treeApply(&data, datafree, NULL);
    treeDtor(&data);

    return SUCCESS;
}

enum MODES getMode(void)
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

enum CODES akinator(tree_t *data)
{
    CHECK(NULL != data, NULLPTRERR);

    if (treeEmpty(data))
    {
        enum CODES status = unknowen(data, DUMMY_CHILD);
        CHECK(SUCCESS == status, status);
    }



    return SUCCESS;
}

enum CODES unknowen(tree_t *data, enum CHILD_CODE node)
{
    CHECK(NULL != data, NULLPTRERR);

    printf("Who/What is it?\n");

    char resp[64] = "";
    scanf("%63s", resp);

    CHECK(TREE_SUCCESS == treeInsert(data, node, strdup(resp)), TREE_OPS_ERROR);
    return SUCCESS;
}

void datafree(char **data, void *dummy)
{
    if ((NULL != data) && (NULL == dummy))
    {
        free(*data);
        *data = NULL;
    }
}

