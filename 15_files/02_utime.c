#define _GNU_SOURCE                             /* S_IFMT, S_IFREG, AT_FDCWD */
#include <utime.h>                              /* utime */
#include <fcntl.h>                              /* AT_FDCWD */
#include <sys/stat.h>                           /* utimensat, futimens */
#include <stdio.h>                              /* printf */
#include <stdlib.h>                             /* strtol */



//  Возвращает 0 при успешном завершении и -1 при ошибке
int utime(const char *pathname, const struct utimbuf *buf);

// struct utimbuf {
//     time_t actime;                      /* Время доступа */
//     time_t modtime;                     /* Время изменения */
// };
// -------------------------------------------------------------

// Возвращает 0 при успешном завершении и -1 при ошибке
int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);
/*
struct timespec {
    time_t tv_sec;
    long tv_nsec;
}; */
// -------------------------------------------------------------

//  Возвращает 0 при успешном завершении и -1 при ошибке
/*  Библиотечная функция futimens() обновляет метки времени файла, на который
    ссылается дескриптор открытого файла fd */
int futimens(int fd, const struct timespec times[2]);



int main(int argc, char *argv[])
{
    struct timespec ts[2];
    char *atime;
    char *mtime;

    ts[0].tv_sec = strtol(argv[2], NULL, 0);
    ts[0].tv_nsec = strtol(argv[3], NULL, 0);

    ts[1].tv_sec = strtol(argv[4], NULL, 0);
    ts[1].tv_nsec = strtol(argv[5], NULL, 0);

    if (utimensat(AT_FDCWD, argv[1], ts, 0) == -1)
        return -1;


    return 0;
}