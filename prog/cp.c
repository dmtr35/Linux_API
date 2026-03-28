#define _GNU_SOURCE
#include <fcntl.h>      /* open */
#include <unistd.h>     /* read, write lseek ftruncate*/
#include <sys/types.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


#define MAX_READ 3

int main(int argc, char *argv[])
{
    char *file, *copy_file;
    int fd_file, fd_copy;
    ssize_t num_read, num_write;
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

    
    while ((num_read = read(fd_file, buffer, MAX_READ)) > 0) {
        int i = 0, count = 0, count_zero = 0;
        for (i = 0; i < num_read; i++) {
            if (buffer[i] == '\0') {
                if (count != 0) {
                    num_write = write(fd_copy, &buffer[i-count], count);
                    if (num_write == -1)
                        errExit("write");
                    count = 0;
                }
                count_zero++;
            } else {
                if (count_zero != 0) {
                    lseek(fd_copy, count_zero, SEEK_CUR);
                    count_zero = 0;
                }
                count++;
            }
        }

        if (count > 0) {
            num_write = write(fd_copy, &buffer[i-count], count);
            if (num_write != count)
                errExit("partial write");
            if (num_write == -1)
                errExit("write");
        } else if (count_zero > 0) {
            off_t pos = lseek(fd_copy, count_zero, SEEK_CUR);
            if (pos == -1)
                errExit("lseek");

            if (ftruncate(fd_copy, pos) == -1)
                errExit("ftruncate");                   /* устанавливает размер файла fd в length байт */
        }
    }

    return 0;
}
