#define _GNU_SOURCE
#include <fcntl.h>              /* open */
#include <sys/uio.h>            /* readv */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

#define MAX_READ 1024

// фрагментированный ввод/вывод (scatter-gather I/O) !!!

// Возвращает количество считанных байтов, 0 при EOF или –1 при ошибке
ssize_t readv(int fd, const struct iovec *iov, int iovcnt);

// Возвращает количество записанных байтов или –1 при ошибке
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);


int main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[3];
    struct stat myStruct;           /* Первый буфер */
    int x;                          /* Второй буфер */
#define STR_SIZE 100
    char str[STR_SIZE];             /* Третий буфер */
    ssize_t numRead, totRequired;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        errExit("open");

    totRequired = 0;
    iov[0].iov_base = &myStruct;
    iov[0].iov_len = sizeof(struct stat);
    totRequired += iov[0].iov_len;
    iov[1].iov_base = &x;
    iov[1].iov_len = sizeof(x);
    totRequired += iov[1].iov_len;
    iov[2].iov_base = str;
    iov[2].iov_len = STR_SIZE;
    totRequired += iov[2].iov_len;

    numRead = readv(fd, iov, 3);
    if (numRead == -1)
        errExit("readv");
    if (numRead < totRequired)
        printf("Read fewer bytes than requested\n");
        printf("total bytes requested: %ld; bytes read: %ld\n", (long) totRequired, (long) numRead);
        
    exit(EXIT_SUCCESS);
}
    

