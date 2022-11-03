#include <stdio.h>
#include "bintree.h"


enum MODES
{
    AKINATOR   = 0,
    DEFINITION = 1,
    DIFFERENCE = 2,
    ERROR      = 3
};

enum CODES
{
    SUCCESS    = 0,
    WRONG_MODE = 1
};


enum MODES getMode(void);

enum CODES play(void);

enum CODES akinator(tree_t *data);


int main(void)
{
    play();

    return 0;
}


enum CODES play(void)
{
    tree_t data = {0};
    treeCtor(&data);

    enum MODES mode = ERROR;
    if ((mode = getMode()) == ERROR)
    {
        return WRONG_MODE;
    }

    switch (mode)
    {
        case AKINATOR:
            akinator(&data);
            break;
        case DEFINITION:

            break;
        case DIFFERENCE:

            break;

        default:
            printf("Undefined mode\n");
            return WRONG_MODE;
    }

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
        }
    }

    return ERROR;
}

enum CODES akinator(tree_t *data)
{
}

