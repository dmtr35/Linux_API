#define _GNU_SOURCE
#include <fcntl.h>          /* open */
#include <unistd.h>         /* read */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


// Возвращает количество считанных байтов, 0 при EOF или –1 при ошибке
// count определяет максимальное количество считываемых байтов
// buffer предоставляет адрес буфера памяти, в который должны быть помещены входные данные
ssize_t read(int fd, void *buffer, size_t count);




#define MAX_READ 5

int main()
{
    char buffer_1[MAX_READ];
    int fd = open("startup", O_RDONLY);
    if (fd == -1)
        errExit("open");


    ssize_t res = read(STDIN_FILENO, buffer_1, MAX_READ);
    if (res == -1)
        errExit("read");
    printf("The input data was: %s\n", buffer_1);

    char buffer_2[MAX_READ + 1];
    ssize_t numRead = read(STDIN_FILENO, buffer_2, MAX_READ);
    if (numRead == -1)
        errExit("read");
    buffer_2[numRead] = '\0';
    printf("The input data was: %s\n", buffer_2);


    return 0;
}







