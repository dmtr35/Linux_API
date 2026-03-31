#define _LARGEFILE64_SOURCE     /* off64_t */
#include <sys/stat.h>           /* S_IRUSR | S_IWUSR */
#include <fcntl.h>              /* off_t */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// для систем x32, работа с файлами больше 2Гб
/* компиляция с макрос _FILE_OFFSET_BITS
gcc -D_FILE_OFFSET_BITS=64 prog.c */

int main(int argc, char *argv[])
{
    int fd;
    off64_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname offset\n", argv[0]);

    fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open64");

    off = atoll(argv[2]);
    if (lseek64(fd, off, SEEK_SET) == -1)
        errExit(lseek64);

    if (write(fd, "test", 4) == -1)
        errExit("write");

    exit(EXIT_SUCCESS);
}




