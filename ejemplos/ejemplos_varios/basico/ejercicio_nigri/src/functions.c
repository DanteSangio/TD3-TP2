#include <stdlib.h>     // free()

#include "header.h"     // main_t


void free_all (main_t *st)
{
    free(st->pid_childs);
}