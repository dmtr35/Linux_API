#define _GNU_SOURCE
#include <fcntl.h>      /* open */
#include <unistd.h>     /* getpid getppid*/
#include <stdio.h>      /* printf */
#include <stdlib.h>
#include <errno.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

#define MAX_READ 30

int main(int argc, char *argv[])
{
    printf("%s\n", program_invocation_name);            // первый аргумент argv[]
    printf("%s\n", program_invocation_short_name);      // имя программы без пути
    // int j;
    // for (j = 0; j < argc; j++)
    //     printf("argv[%d] = %s\n", j, argv[j]);
    // ==========================================
    // char **p;
    // for (p = argv; *p != NULL; p++)
    //     puts(*p);
    // ==========================================

    int fd;
    pid_t pid;
    size_t size_pid, size_path;
    char *path;
    ssize_t num_read, num_write;
    char buf[MAX_READ + 1];
    char str_pid[10];

    pid = getpid();
    sprintf(str_pid, "%d", pid);

    size_pid = strlen(str_pid);
    size_path = size_pid + 15;

    path = malloc(size_path);
    snprintf(path, size_path, "/proc/%s/cmdline", str_pid);

    fd = open(path, O_RDONLY);
    if (fd == -1)
        errExit("open");

    // while(true)
    while((num_read = read(fd, buf, MAX_READ)) > 0) {
        for (size_t i = 0; i < num_read; i++) {
            if (buf[i] == '\0')
                buf[i] = '\n';
        }
        num_write = write(STDOUT_FILENO, buf, num_read);
        if (num_write == -1)
            errExit("write");
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}
