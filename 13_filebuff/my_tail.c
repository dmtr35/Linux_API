#include <stdio.h>                              /* printf */
#include <sys/stat.h>                           /* open: "S_IRUSR, S_IWUSR" */
#include <fcntl.h>                              /* open */
#include <string.h>                             /* strcmp */
#include <stdlib.h>                             /* strtol */
#include <unistd.h>                             /* read, ssize_t, STDOUT_FILENO */
#include "../lib/error_functions.h"

#define MAX_READ 3

int main(int argc, char *argv[])
{
    int fd, buf_size = MAX_READ;
    int n = 10, flag_f = 1;
    ssize_t res_read, res_write;
    char *file, *buf;
    off_t offset = 0, p;

    if (argc < 2) {
        usageErr("%s [ -f ] [ -n num ] file\n", argv[0]);
    } else {
        file = argv[argc - 1];
        for(int i = 1; i < argc - 1; ++i) {
            char *arg = argv[i];

            if (strcmp(arg, "-f") == 0) {
                flag_f = 0;
                continue;
            }

            if (strncmp(arg, "-n", 2) == 0) {
                if (strcmp(arg, "-n") == 0) {
                    n = strtol(argv[++i], NULL, 10);
                    continue;
                }
                n = strtol(argv[i] + 2, NULL, 10);
            }
        }
    }

    // ----------------------------------------------------

    fd = open(file, O_RDONLY);
    if (fd == -1)
        errExit("open");

    buf = malloc(buf_size);
    if (buf == NULL)
        errExit("malloc");

    while ((res_read = read(fd, buf, MAX_READ)) > 0) {
        if (res_read == -1)
            errExit("read");
        
        buf[res_read] = '\0';

        p = strcspn(buf, "\n");
        if (buf[p] == '\n') {
            offset += p;
            lseek(fd, ++offset, SEEK_SET);
        }
        
        res_write = write(STDOUT_FILENO, buf, ++p);
    }


    return 0;
}