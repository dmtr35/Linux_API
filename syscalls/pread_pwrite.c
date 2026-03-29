#define _GNU_SOURCE
#include <fcntl.h>              /* open */
#include <unistd.h>             /* pread pwrite */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"
#define MAX_READ 1024

// вызовы pread, pwriteне изменяют файлового смещения !!!

// Возвращает количество считанных байтов, 0 при EOF или –1 при ошибке
ssize_t pread(int fd, void *buf, size_t count, off_t offset);

// Возвращает количество записанных байтов или –1 при ошибке
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);


int main(int argc, char *argv[])
{
    int fd;
    off_t size;
    ssize_t num_read, num_write;
    char buffer[MAX_READ];
    char *line = "one line\n";
    char *file = argv[1];

    fd = open(file, O_RDWR);
    if (fd == -1)
        errExit("open");

    size = lseek(fd, 0, SEEK_END);
    num_read = pread(fd, buffer, size-3, 3);
    if (num_read == -1)
        errExit("pread");

    num_write = pwrite(fd, line, strlen(line), size);
    if (num_write == -1)
        errExit("num_write");


    return 0;
}
