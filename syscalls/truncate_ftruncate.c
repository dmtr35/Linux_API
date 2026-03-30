#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// Усечение файла или добавление нулей в конец:
// Оба возвращают 0 при успешном завершении или –1 при ошибке
int truncate(const char *pathname, off_t length);
int ftruncate(int fd, off_t length);


int main(int argc, char *argv[])
{
    int fd, res;
    off_t length = 8;
    char *file = argv[1];

    fd = open(file, O_RDWR);
    if (fd == -1)
        errExit("open");

    res = ftruncate(fd, length);
    if (res == -1)
        errExit("ftruncate");
        
    res = close(fd);
    if (res == -1)
        errExit("close");

    // не требует открывать файл с open:
    res = truncate(file, 15);
    if (res == -1)
        errExit("truncate");

}

