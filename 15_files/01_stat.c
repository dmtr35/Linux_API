#define _GNU_SOURCE                     /* S_IFMT, S_IFREG */
#include <sys/stat.h>                   /* stat, lstat, fstat */
#include <stdio.h>                      /* printf */
#include <sys/stat.h>                   /* open: "S_IRUSR, S_IWUSR" */
#include <fcntl.h>                      /* open, O_RDONLY */
#include <unistd.h>                     /* close */
#include "../lib/error_functions.h"

//  Все вызовы возвращают 0 при успешном завершении и -1 при ошибке
//  stat() возвращает информацию об именованном файле;
int stat(const char *pathname, struct stat *statbuf);

//  если именованный файл является символической ссылкой, то возвращается информация о самой ссылке, а не о файле, накоторую она указывает;
int lstat(const char *pathname, struct stat *statbuf);

//  возвращает информацию о файле, к которому обращается открытый файловый дескриптор.
int fstat(int fd, struct stat *statbuf);


void printf_stat(struct stat *statbuf)
{
    printf(
        "%-10s: %ld\n"
        "%-10s: %ld\n"
        "%-10s: %d\n"
        "%-10s: %ld\n"
        "%-10s: %d\n"
        "%-10s: %d\n"
        "%-10s: %ld\n"
        "%-10s: %ld\n"
        "%-10s: %ld\n"
        "%-10s: %ld\n"
        "%-10s: %ld\n"
        "%-10s: %ld\n"
        "%-10s: %ld\n",
        "st_dev", statbuf->st_dev,          // Идентификатор устройства, на котором находится файл      st_dev    : 64769                             
        "st_ino", statbuf->st_ino,          // Номер индексного дескриптора файла                       st_ino    : 5776159             
        "st_mode", statbuf->st_mode,        // Тип файла и права доступа                                st_mode   : 33204 
        "st_nlink", statbuf->st_nlink,      // Количество (жестких) ссылок на файл                      st_nlink  : 1             
        "st_uid", statbuf->st_uid,          // Пользовательский ID владельца файла                      st_uid    : 1000             
        "st_gid", statbuf->st_gid,          // Групповой ID владельца файла                             st_gid    : 1000     
        "st_rdev", statbuf->st_rdev,        // Идентификаторы файлов устройств                          st_rdev   : 0         
        "st_size", statbuf->st_size,        // Общий размер файла (в байтах)                            st_size   : 32     
        "st_blksize", statbuf->st_blksize,  // Оптимальный размер блока для ввода-вывода (в байтах)     st_blksize: 4096                             
        "st_blocks", statbuf->st_blocks,    // Количество отведенных блоков (по 512 байт)               st_blocks : 8                     
        "st_atime", statbuf->st_atime,      // Время последнего доступа к файлу                         st_atime  : 1782223799         
        "st_mtime", statbuf->st_mtime,      // Время последнего изменения файла                         st_mtime  : 1781455645         
        "st_ctime", statbuf->st_ctime       // Время последнего изменения статуса                       st_ctime  : 1781455645             
    );
}

/*  Макросы для проверки типов файлов в поле st_mode структуры stat
    Константа           Проверочный макрос          Тип файла
    S_IFREG             S_ISREG()                   Обычный файл
    S_IFDIR             S_ISDIR()                   Каталог
    S_IFCHR             S_ISCHR()                   Символьное устройство
    S_IFBLK             S_ISBLK()                   Блочное устройство
    S_IFIFO             S_ISFIFO()                  Очередь FIFO или канал
    S_IFSOCK            S_ISSOCK()                  Сокет
    S_IFLNK             S_ISLNK()                   Символическая ссылка
*/

int main(int argc, char *argv[])
{
    int res, fd;
    struct stat sb;
    
    res = stat("startup", &sb);
    printf_stat(&sb);

    if ((sb.st_mode & S_IFMT) == S_IFREG)
        printf("regular file\n");
    // Поскольку данная операция довольно обычная, для упрощения написанного выше применяется стандартный макрос:
    if (S_ISREG(sb.st_mode))
        printf("regular file\n");

    // -----------------------------------------------
    
    printf("\n");
    res = lstat("link_startup", &sb);
    printf_stat(&sb);
    // -----------------------------------------------

    printf("\n");
    fd = open("startup", O_RDONLY);
    if(fd == -1)
        errExit("open");

    res = fstat(fd, &sb);
    printf_stat(&sb);

    if (close(fd) == -1)
        errExit("close");

    // ===============================================





    
    return 0;
}
