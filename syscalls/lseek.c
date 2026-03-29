#define _GNU_SOURCE
#include <fcntl.h>              /* open */
#include <unistd.h>             /* read, write, lseek */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


// Возвращает новое файловое смещение при успешном завершении или –1 при ошибке
// off_t — целочисленный тип со знаком
// Аргумент offset определяет значение смещения в байтах

// whence указывает на отправную точку, от которой отсчитывается смещение:
// SEEK_SET — смещение в байтах на расстоянии offset от начала файла
// SEEK_CUR — смещение в байтах на расстоянии offset относительно текущего файлового смещения;
// SEEK_END — файловое смещение устанавливается на размер файла плюс offset
off_t lseek(int fd, off_t offset, int whence);

#define MAX_READ 5

int main()
{
    // SEEK_SET     всегда от начала файла
    off_t offset = 2;
    char buffer_1[MAX_READ + 1];

    int fd = open("startup", O_RDONLY);
    if (fd == -1)
        errExit("open");

    read(fd, buffer_1, 1);
    off_t c = lseek(fd, offset, SEEK_SET);

    ssize_t numRead = read(fd, buffer_1, MAX_READ);
    if (numRead == -1)
        errExit("read");
    buffer_1[numRead] = '\0';
    printf("Buffer: %s\n", buffer_1);

    if (close(fd) == -1)
        errExit("close");
// ---------------------------------------------------------
    // SEEK_END         с конца файла
    offset = -2;
    char buffer_2[MAX_READ + 1];
    
    fd = open("startup", O_RDONLY);
    if (fd == -1)
        errExit("open");
    
    off_t size = lseek(fd, 0, SEEK_END);    /* Узнать размер файла */
    c = lseek(fd, offset, SEEK_END);

    numRead = read(fd, buffer_2, MAX_READ);
    if (numRead == -1)
        errExit("read");
    buffer_2[numRead] = '\0';
    printf("Buffer: %s\n", buffer_2);
    
    if (close(fd) == -1)
        errExit("close");
// ---------------------------------------------------------
    // SEEK_CUR         от текущей позиции
    offset = 2;
    char buffer_3[MAX_READ + 1];
    
    fd = open("startup", O_RDONLY);
    if (fd == -1)
        errExit("open");
    
    read(fd, buffer_3, 1);
    c = lseek(fd, offset, SEEK_CUR);

    numRead = read(fd, buffer_3, MAX_READ);
    if (numRead == -1)
        errExit("read");
    buffer_2[numRead] = '\0';
    printf("Buffer: %s\n", buffer_3);
    
    if (close(fd) == -1)
        errExit("close");
// ===========================================================
    offset = 50;
    char buffer[50];
    char buffer_4[] = " hello_world!\n";    

    fd = open("startup", O_RDWR);
    if (fd == -1)
        errExit("open");

    // c = lseek(fd, 0, SEEK_END);             /* сдвиг в конец файла */
    c = lseek(fd, offset, SEEK_SET);
    ssize_t w = write(fd, buffer_4, 14);             /* будет создана дыра */
    if (w == -1)
        errExit("write");

    c = lseek(fd, 0, SEEK_SET);
    ssize_t res = read(fd, buffer, 49);
    if (res == -1)
        errExit("read");
    buffer[res] = '\0';
    
    printf("Buffer: %s\n", buffer);


    return 0;
}







