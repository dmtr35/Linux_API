#define _GNU_SOURCE /* Для получения различных объявлений из <stdlib.h> */
#include <stdlib.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

extern char **environ;

//  ./env_modify "GREET=Guten Tag" SHELL=/bin/bash BYE=Ciao
// SHELL=/bin/sh BYE=byebye
int main(int argc, char *argv[])
{
    int j;
    char **ep;

    clearenv();             /* Удаление всей среды */

    for (j = 1; j < argc; j++)
        if (putenv(argv[j]) != 0)           // добавляем значания в среду
            errExit("putenv: %s", argv[j]);

    if (setenv("GREET", "Hello world", 0) == -1)    // пытается добавить значение в среду
        errExit("setenv");

    unsetenv("BYE");                                // удаляет значение из среды

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    exit(EXIT_SUCCESS);
}

