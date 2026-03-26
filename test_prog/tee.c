#include <fcntl.h>      /* open */
#include <unistd.h>     /* read write lseek */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


int main(int argc, char *argv[])
{
    int fd;
    char *path;

    if (argc < 2 || argc > 3 || (argc == 3 && strcmp(argv[1], "-a") != 0))
        usageErr("%s [-a] file\n", argv[0]);



    return 0;
}