#include <stdio.h>
#include "bintree.h"


enum MODES
{
    AKINATOR   = 0,
    DEFINITION = 1,
    DIFFERENCE = 2,
    ERROR      = 3
};


enum MODES getMode(void);

void play(void);

void akinator(tree_t *data);


int main(void)
{
    play();

    return 0;
}


void play(void)
{
    tree_t data = {0};
    treeCtor(&data);

    enum MODES mode = ERROR;
    if ((mode = getMode()) == ERROR)
    {
        return;
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
            return;
    }

    treeDtor(&data);
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

void akinator(tree_t *data)
{
}

