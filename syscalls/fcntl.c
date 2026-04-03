#include <fcntl.h>
#include <string.h>             /* strlen */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

#define MAX_READ 1024

// Операции управления файлом: fcntl()
/* Значение, возвращаемое при успешном завершении,
зависит от значения cmd или равно –1 при сбое */
int fcntl(int fd, int cmd, ...);

// F_GETFL - извлечение флагов режима доступа и состояния открытого файла
// F_SETFL - изменение некоторых флагов состояния открытого файла (O_APPEND, O_NONBLOCK, O_NOATIME, O_ASYNC, O_DIRECT) 


int main(int argc, char *argv[])
{
    /* Флаги состояния открытого файла
    Извлечение или изменение флагов режима
    доступа и состояния открытого файла */
    int fd, cl, flags, accessMode;
    char *file = argv[1];
    char first_buf[] = "First_1001";
    char second_buf[] = "Hello_world!";
    ssize_t num_write;
    

    fd = open(file, O_WRONLY | O_CREAT | O_SYNC, 0644);
    if (fd == -1)
        errExit("open");

    num_write = write(fd, first_buf, strlen(first_buf));
    if (num_write == -1)
        errExit("write");

    flags = fcntl(fd, F_GETFL);     /* Третий аргумент не требуется */
    if (flags == -1)
        errExit("fcntl");

    /* был ли файл открыт для синхронизированной записи: */
    if (flags & O_SYNC)
        printf("записи синхронизированы \n");

    /* Проверка режима доступа к файлу */
    accessMode = flags & O_ACCMODE;
    if (accessMode == O_WRONLY || accessMode == O_RDWR)
        printf("file is writable\n");
// ================================================================

    // чтобы включить O_APPEND у открытого файла для записи в конец файла (flags & ~O_APPEND; - выключить)
    flags |= O_APPEND;
    if (fcntl(fd, F_SETFL, flags) == -1)
        errExit("fcntl");

    num_write = write(fd, second_buf, strlen(second_buf));
    if (num_write == -1)
        errExit("write");

    

    cl = close(fd);
    if (cl == -1)
        errExit("res");
    
    return 0;
}