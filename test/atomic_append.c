#include <fcntl.h>                  /* open */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


int main(int argc, char *argv[])
{
    int fd;
    ssize_t num_write;
    off_t pos;
    char c = argv[3][0];

    char *file = argv[1];
    int num_bytes = strtol(argv[2], 0, 0);

    int use_append = 1;

    if (argc == 5 && strcmp(argv[4], "x") == 0) {
        use_append = 0;
    }

    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC | (use_append ? O_APPEND : 0), 0644);
    if (fd == -1)
        errExit("open");

    for (int i = 0; i < num_bytes; i++) {
        if (!use_append){
            if (write(fd, &c, 1) != 1)
                errExit("write");
        } else {
            pos = lseek(fd, 0, SEEK_END);
            if (pos == -1)
                errExit("lseek");
            if (write(fd, &c, 1) != 1)
                errExit("write");
        }
    }

    if (close(fd) == -1)
        errExit("close");
    

    return 0;
}
