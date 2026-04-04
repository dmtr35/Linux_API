#define _GNU_SOURCE
#include <fcntl.h>                  /* open */
#include <unistd.h>                 /* dup */
#include <stdio.h>                  /* printf */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// Операции управления файлом: fcntl()
/* Значение, возвращаемое при успешном завершении,
зависит от значения cmd или равно –1 при сбое */
int fcntl(int fd, int cmd, ...);
// F_GETFL - извлечение флагов режима доступа и состояния открытого файла
// F_SETFL - изменение некоторых флагов состояния открытого файла (O_APPEND, O_NONBLOCK, O_NOATIME, O_ASYNC, O_DIRECT) 



int check_fds(int fd_fir, int fd_sec)
{
    int orig_flags, ch_flags, saved_errno;
    off_t orig_pos, fir_pos, sec_pos;

    if ((orig_pos = lseek(fd_fir, 0, SEEK_CUR)) == -1)
        return -1;

    fir_pos = orig_pos;

    if ((sec_pos = lseek(fd_sec, 0, SEEK_CUR)) == -1)
        return -1;

    if (fir_pos != sec_pos)
        return 1;

    if ((fir_pos = lseek(fd_fir, 0, SEEK_END)) == -1) {
        saved_errno = errno;
        lseek(fd_fir, orig_pos, SEEK_SET);
        errno = saved_errno;
        return -1;
    }

    if ((sec_pos = lseek(fd_sec, 0, SEEK_CUR)) == -1) {
        saved_errno = errno;
        lseek(fd_fir, orig_pos, SEEK_SET);
        errno = saved_errno;
        return -1;
    }

    if (fir_pos != sec_pos) {
        lseek(fd_fir, orig_pos, SEEK_SET);
        return 1;
    }

    if (lseek(fd_fir, orig_pos, SEEK_SET) == -1)
        return -1;
    
    // ==============================================================
    orig_flags = fcntl(fd_fir, F_GETFL);
    if (orig_flags == -1)
        return -1;

    if (fcntl(fd_fir, F_SETFL, orig_flags | O_APPEND) == -1) {
        saved_errno = errno;
        fcntl(fd_fir, F_SETFL, orig_flags);
        errno = saved_errno;
        return -1;
    }

    if (fcntl(fd_fir, F_GETFL) != fcntl(fd_sec, F_GETFL)) {
        fcntl(fd_fir, F_SETFL, orig_flags);
        return 1;
    }

    if (fcntl(fd_fir, F_SETFL, orig_flags & ~O_APPEND) == -1) {
        saved_errno = errno;
        fcntl(fd_fir, F_SETFL, orig_flags);
        errno = saved_errno;
        return -1;
    }

    if (fcntl(fd_fir, F_GETFL) != fcntl(fd_sec, F_GETFL)) {
        fcntl(fd_fir, F_SETFL, orig_flags);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int fd, new_fd, res;
    char *file = argv[1];

    fd = open(file, O_RDONLY);
    if (fd == -1)
        errExit("open");

    new_fd = dup(fd);
    if (new_fd == -1)
        errExit("dup");

    res = check_fds(fd, new_fd);
    if (res == -1)
        errExit("check_fds");             // ошибка системного вызова
    else
        printf("%s\n", (res == 0) ? "fds the same" : "fds not same");
}

