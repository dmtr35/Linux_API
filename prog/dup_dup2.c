#include <fcntl.h>                  /* open */
// #include <unistd.h>                 /* dup */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"



// Операции управления файлом: fcntl()
/* Значение, возвращаемое при успешном завершении,
зависит от значения cmd или равно –1 при сбое */
int fcntl(int fd, int cmd, ...);


int my_dup(int oldfd)
{
    return fcntl(oldfd, F_DUPFD, 0);
}

int my_dup2(int old_fd, int new_fd)
{
    if (old_fd == new_fd)
        if (fcntl(old_fd, F_GETFL) == -1)
            return -1;
        return new_fd;

    if (fcntl(old_fd, F_GETFL) == -1)
        return -1;

    if (close(new_fd) == -1 && errno != EBADF)
        return -1;

    return fcntl(old_fd, F_DUPFD, new_fd);
}

int main(int argc, char *argv[])
{
    int fd, new_fd;
    ssize_t num_write, num_read;
    char *file = argv[1];
    char *buffer = "hello_world!";

    fd = open(file, O_RDWR);
    if (fd == -1)
        errExit("open");

    // ------------------------------------------

    // new_fd = my_dup(fd);
    // if (new_fd == -1)
    //     errExit("my_dup");
    new_fd = my_dup2(fd, 2);
    if (new_fd == -1)
        errExit("my_dup2");

    num_write = write(new_fd, buffer, strlen(buffer));
    if (num_write == -1)
        errExit("write");
    
}