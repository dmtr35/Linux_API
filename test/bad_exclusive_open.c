#include <fcntl.h>
#include <unistd.h>             /* getpid */
#include <sys/stat.h>           /* S_IRUSR | S_IWUSR */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"



int main(int argc, char *argv[])
{
    int fd;
    char *file = argv[1];

    fd = open(file, O_WRONLY); /* Открытие 1: проверка существования файла */
    if (fd != -1) {
        /* Открытие прошло успешно */
        printf("[PID %ld] File \"%s\" already exists\n", (long) getpid(), file);
        close(fd);
    } else {
        if (errno != ENOENT) {
            /* Сбой по неожиданной причине */
            errExit("open");
        } else {
            /* ОТРЕЗОК ВРЕМЕНИ НА СБОЙ */
            fd = open(file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1)
                errExit("open");
            printf("[PID %ld] Created file \"%s\" exclusively\n", (long) getpid(), file);      /* МОЖЕТ БЫТЬ ЛОЖЬЮ! */
        }
    }

    return 0;
}