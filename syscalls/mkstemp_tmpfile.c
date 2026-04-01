#define _GNU_SOURCE
// #include <fcntl.h>          /* open */
#include <stdlib.h>         /* mkstemp */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// При успешном завершении возвращает новый файловый дескриптор, а при ошибке выдает –1
int mkstemp(char *template);

int main(int argc, char *argv[])
{
    int fd;
    char template[] = "/tmp/somestring_XXXXXX";

    fd = mkstemp(template);
    if (fd == -1)
        errExit("mkdtemp");

    // template изменился:
    printf("Generated filename was: %s\n", template);
    unlink(template);   /* Имя тут же исчезает, но файл удаляется только после close() */

    if (close(fd) == -1)
        errExit("close");
}





// #include <stdio.h>
// Возвращает указатель на файл при успешном завершении или NULL при ошибке
FILE *tmpfile(void);