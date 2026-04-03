#define _GNU_SOURCE
#include <fcntl.h>                  /* open */
#include <unistd.h>                 /* dup */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// Операции управления файлом: fcntl()
/* Значение, возвращаемое при успешном завершении,
зависит от значения cmd или равно –1 при сбое */
int fcntl(int fd, int cmd, ...);
// F_GETFL - извлечение флагов режима доступа и состояния открытого файла
// F_SETFL - изменение некоторых флагов состояния открытого файла (O_APPEND, O_NONBLOCK, O_NOATIME, O_ASYNC, O_DIRECT) 

int check_fds(int fd_old, int fd_new)
{
    int orig_flags, ch_flags, ret = -1;
    off_t orig_pos, old_pos, new_pos;

    if ((orig_pos = lseek(fd_old, 0, SEEK_CUR)) == -1)
        return -1;

    old_pos = orig_pos;
    if ((new_pos = lseek(fd_new, 0, SEEK_CUR)) == -1)
        return -1;

    if (old_pos != new_pos)
        return -1;

    if ((old_pos = lseek(fd_old, 0, SEEK_END)) == -1)
        goto restore_offset;
    if ((new_pos = lseek(fd_new, 0, SEEK_CUR)) == -1)
        goto restore_offset;
    if (old_pos != new_pos)
        goto restore_offset;

    if (lseek(fd_old, orig_pos, SEEK_SET) == -1)
        return -1;
    
    // ==============================================================
    check_flags:
    orig_flags = fcntl(fd_old, F_GETFL);
    if (orig_flags == -1)
        return -1;

    if (fcntl(fd_old, F_SETFL, orig_flags | O_APPEND) == -1)
        goto restore_flags;
    if (fcntl(fd_old, F_GETFL) != fcntl(fd_new, F_GETFL))
        goto restore_flags;

    if (fcntl(fd_old, F_SETFL, orig_flags & ~O_APPEND) == -1)
        goto restore_flags;
    if (fcntl(fd_old, F_GETFL) != fcntl(fd_new, F_GETFL))
        goto restore_flags;
    

    ret = 0;
    restore_flags:
    if (fcntl(fd_old, F_SETFL, orig_flags) == -1)
        return -1;
    return ret;
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
        errExit("res");
}

