#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>             /* dup */
#include <string.h>             /* strlen */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

#define MAX_READ 1024

// При успешном завершении возвращает новый файловый дескриптор, а при ошибке выдает –1
int dup(int oldfd);
int dup2(int oldfd, int newfd);
int dup3(int oldfd, int newfd, int flags);
// dup3(oldfd, newfd, O_CLOEXEC);


int main(int argc, char *argv[])
{
    int fd, fd_new;
    char buffer[] = "successful\n";
    char buffer_ERROR[] = "Error 404\n";
    char *file = argv[1];
    ssize_t num_read, num_write;
    off_t size;
    int startfd = 10;

    fd = open(file, O_RDWR | O_CREAT | O_TRUNC);
    if (fd == -1)
        errExit("open");

    // ---------------------------------
    // close(1);
    // dup(fd);
    // close(2);
    // dup(fd);
    // ---------------------------------
    
    // dup2(fd, 1);
    // dup2(fd, 2);
    // ---------------------------------
    // дублирования файловых дескрипторов
    // дескриптор будет равен или больше startfd
    // fd_new = fcntl(fd, F_DUPFD, startfd);
    fd_new = fcntl(fd, F_DUPFD_CLOEXEC, startfd); /* устанавливает фраг закрытия FD_CLOEXEC */


    num_write = write(fd_new, buffer, strlen(buffer));
    if (num_write == -1)
        errExit("num_write");

    num_write = write(STDOUT_FILENO, buffer, strlen(buffer));
    if (num_write == -1)
        errExit("num_write");

    num_write = write(STDERR_FILENO, buffer_ERROR, strlen(buffer_ERROR));
    if (num_write == -1)
        errExit("num_write");



    return 0;
}

