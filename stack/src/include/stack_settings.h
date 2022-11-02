#ifndef __STACK_SETTINGS_H__
#define __STACK_SETTINGS_H__



#include <math.h>



#define ELEM_POISON NULL
#define IS_ELEM_POISON(elem) (ELEM_POISON == (elem))

#define COLORFULL

#define PRINTELEM(PFILE, ELEM) fprintf(PFILE, "%p", (void *) ELEM)


typedef char *elem_t;



#endif /* __STACK_SETTINGS_H__ */

