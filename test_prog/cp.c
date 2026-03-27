#define _GNU_SOURCE
#include <fcntl.h>      /* open */
#include <unistd.h>     /* read, write */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


#define MAX_READ 1024

int main(int argc, char *argv[])
{
    char *file, *copy_file;
    int fd_file, fd_copy, num_read, num_write;
    char buffer[MAX_READ + 1];

    if (argc < 3)
        usageErr("%s file copy_file\n", argv[0]);

    file = argv[1];
    copy_file = argv[2];

    fd_file = open(file, O_RDONLY);
    if (fd_file == -1)
        errExit("open");

    fd_copy = open(copy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_copy == -1)
        errExit("open");

    

    num_read = read(fd_file, buffer, MAX_READ);
    if (num_read == -1)
        errExit("read");
        
    num_write = write(fd_file, buffer, num_read);
    if (num_write == -1)
        errExit("write");

    printf("%s\n", buffer);

    return 0;
}
