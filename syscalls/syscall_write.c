#define _GNU_SOURCE
#include <fcntl.h>              /* open */
#include <unistd.h>             /* read */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


// Возвращает количество записанных байтов или –1 при ошибке
ssize_t write(int fd, const void *buffer, size_t count);




#define MAX_READ 5

int main()
{
    char buffer_1[] = "hello world!\n";
    int fd = open("startup", O_WRONLY);
    if (fd == -1)
        errExit("open");


    ssize_t res1 = write(STDOUT_FILENO, buffer_1, MAX_READ);
    if (res1 == -1)
        errExit("write");
    printf("The input data was: %s\n", buffer_1);

    ssize_t res2 = write(fd, buffer_1, MAX_READ);
    if (res2 == -1)
        errExit("write");
    printf("The input data was: %s\n", buffer_1);



    return 0;
}







