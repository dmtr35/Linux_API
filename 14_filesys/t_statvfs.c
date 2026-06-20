#include <sys/statvfs.h>            /* statvfs, fstatvfs */
#include <stdio.h>                  /* printf */
#include <stdlib.h>                 /* EXIT_SUCCESS */

/*  функции statvfs() и fstatvfs() получают информацию о смонтированной файловой системе.*/
//  Обе функции возвращают 0 при успешном завершении и -1 при ошибке
int statvfs(const char *pathname, struct statvfs *statvfsbuf);
int fstatvfs(int fd, struct statvfs *statvfsbuf);


//  Демонстрация использования функции statvfs() для получения информации о смонтированной файловой системе.
int main(int argc, char *argv[])
{
    struct statvfs sb;

    if (statvfs("/dev/shm", &sb) == -1) {
        perror("statvfs");
        return 1;
    }

    printf("Block size                       %lu\n", sb.f_bsize);                           /* Размер блока файловой системы (в байтах) */
    printf("Fundamental block size           %lu\n", sb.f_frsize);                          /* Фундаментальный размер блока файловой системы (в байтах) */
    printf("Total blocks (in above units)    %lu\n", (unsigned long) sb.f_blocks);          /* Общее количество блоков в файловой системе (в единицах 'f_frsize') */
    printf("Free blocks for priv. proc.      %lu\n", (unsigned long) sb.f_bfree);           /* Общее количество свободных блоков */
    printf("Free blocks for unpriv. proc.    %lu\n", (unsigned long) sb.f_bavail);          /* Количество свободных блоков, доступных для непривилегированного процесса */
    printf("Total number of i-nodes          %lu\n", (unsigned long) sb.f_files);           /* Общее количество индексных дескрипторов */
    printf("Free i-nodes for priv. proc.     %lu\n", (unsigned long) sb.f_ffree);           /* Общее количество свободных индексных дескрипторов */
    printf("Free i-nodes for nonpriv. proc.  %lu\n", (unsigned long) sb.f_favail);          /* Количество индексных дескрипторов, доступных для непривилегированного процесса (задается в 'f_ffree' в Linux) */
    printf("File system ID                   %#lx\n", sb.f_fsid);                           /* Идентификатор файловой системы */
    printf("Flags                            %#lx\n", sb.f_flag);                           /* Флаги монтирования */
    printf("Maximum filename length          %lu\n", sb.f_namemax);                         /* Максимальная длина имен файлов для данной файловой системы */

    exit(EXIT_SUCCESS);
}
