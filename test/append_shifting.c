#include <fcntl.h>                  /* open */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

int fcntl(int fd, int cmd, ...);
// F_GETFL - извлечение флагов режима доступа и состояния открытого файла
// F_SETFL - изменение некоторых флагов состояния открытого файла (O_APPEND, O_NONBLOCK, O_NOATIME, O_ASYNC, O_DIRECT) 

int main(int argc, char *argv[])
{
    int fd, flags;
    off_t c;
    ssize_t num_write;
    char *buffer = "hello_world!";
    char *file = argv[1];

    fd = open(file, O_WRONLY | O_APPEND, 0644);
    if (fd == -1)
        errExit("open");

    c = lseek(fd, 0, SEEK_SET);
    if (c == -1)
        errExit("lseek");

    num_write = write(fd, buffer, strlen(buffer));
    if (num_write == -1)
        errExit("num_write");
        
    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
        errExit("fcntl");
        
    flags ^= O_APPEND;
    if (fcntl(fd, F_SETFL, flags) == -1)
        errExit("fcntl");

    num_write = write(fd, buffer, strlen(buffer));
    if (num_write == -1)
        errExit("num_write");
}
