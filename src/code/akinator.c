#include "akinator.h"


/*(===========================================================================*/
static enum MODES getmode(void);

static enum CODES akinator(tree_t *data);

static enum CODES unknowen(tree_t *data);

static char *readdatabase(const char *dbname, tree_t *dest);

static long long getfilesize(const char *filename);
/*)===========================================================================*/


/*(===========================================================================*/
enum CODES play(void)
{
    tree_t database = {0};
    CHECK(TREE_SUCCESS == treeCtor(&database), TREE_OPS_ERROR);
    char *datatext = readdatabase("data.txt", &database);
    CHECK(NULL != datatext, DATABASE_ERROR);

    enum MODES mode = getmode();
    switch (mode)
    {
        case AKINATOR:
            break;
        case DEFINITION:

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

    return SUCCESS;
}

static enum CODES unknowen(tree_t *data)
{
    CHECK(NULL != data, NULLPTR_ERROR);

    return SUCCESS;
}

static char *readdatabase(const char *dbname, tree_t *dest)
{
    CHECK(NULL != dbname, NULL);
    CHECK(NULL != dest, NULL);

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

    return data;
}

static long long getfilesize(const char *filename)
{
    CHECK(NULL != filename, LONG_MIN);

    struct stat buf = {0};
    CHECK(0 == stat(filename, &buf), -1);

    return buf.st_size;
}
/*)===========================================================================*/
