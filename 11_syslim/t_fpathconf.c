#include "../lib/tlpi_hdr.h"

//  Использование fpathconf()

/* Выводит 'msg' плюс значение fpathconf(fd, name) */
static void fpathconfPrint(const char *msg, int fd, int name)
{
    long lim;

    errno = 0;
    lim = fpathconf(fd, name);

    if (lim != -1) {                    /* Вызов прошел успешно, ограничение определено */
        printf("%s %ld\n", msg, lim);
    } else {
        if (errno == 0)                 /* Вызов прошел успешно, ограничение не определено */
        printf("%s (indeterminate)\n", msg);
    else                                /* Вызов не удался */
        errExit("fpathconf %s", msg);
    }
}

int main(int argc, char *argv[])
{
    fpathconfPrint("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);       // 255
    fpathconfPrint("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);       // 4096
    fpathconfPrint("_PC_PIPE_BUF: ", STDIN_FILENO, _PC_PIPE_BUF);       // 4096

    exit(EXIT_SUCCESS);
}
