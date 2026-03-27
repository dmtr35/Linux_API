#define _GNU_SOURCE
#include <fcntl.h>      /* open */
#include <unistd.h>     /* read write lseek */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

#define MAX_READ 1024

int main(int argc, char *argv[])
{
    int flag = 0;
    char *file_name;
    char buffer[MAX_READ + 1];
    ssize_t num_read, num_write;
    int file_fd;

    if (argc < 2 || argc > 3 || (argc == 3 && strcmp(argv[1], "-a") != 0))
        usageErr("%s [-a] file\n", argv[0]);

    if (strcmp(argv[1], "-a") == 0) {
        flag = 1;
        file_name = argv[2];
    } else {
        file_name = argv[1];
    }
    
    
    if (flag) {
        file_fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (file_fd == -1)
            errExit("open");
    } else {
        file_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file_fd == -1)
            errExit("open");
    }

    
    while((num_read = read(STDIN_FILENO, buffer, MAX_READ)) > 0) {
        num_write = write(file_fd, buffer, num_read);
        if (num_write == -1)
            errExit("write");
        if (flag) {
            num_write = write(STDOUT_FILENO, buffer, num_read);
            if (num_write == -1)
            errExit("write");
        }
    }
    

    return 0;
}