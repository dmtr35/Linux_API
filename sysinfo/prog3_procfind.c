#include <stdio.h>                      /* printf, snprintf */
#include <unistd.h>                     /* readlink */
#include <dirent.h>                     /* opendir, readdir */
#include <stdlib.h>                     /* strtol */
#include <string.h>                     /* strncmp */
#include "../lib/tlpi_hdr.h"
#include "../lib/error_functions.h"     /* err */

//  Возвращает количество байтов, помещенных в массиве buffer, при успешном завершении или -1 при ошибке
/*  Извлечь содержимое самой ссылки, то есть имя пути, с которым она соотносится
    bufsiz является целым числом, которое используется, чтобы сообщить
    системному вызову readlink() количество байтов, доступных в массиве buffer.
*/
ssize_t readlink(const char *pathname, char *buffer, size_t bufsiz);

//  Возвращает описатель потока каталога или NULL при ошибке
/*  Функция opendir() открывает каталог и возвращает описатель,
который можно применять для ссылки на каталог в последующих вызовах */
DIR *opendir(const char *dirpath);

//  Функция readdir() считывает последовательные записи из потока каталога.
/*  Возвращает указатель на статически выделенную структуру, описывающую
следующую запись каталога, или NULL при достижении конца каталога или при ошибке */
struct dirent *readdir(DIR *dirp);

#define MAX_READ 256

int main(int argc, char *argv[])
{
    uid_t user_uid;
    DIR *info_dir;
    struct dirent *dirent;
    char *endptr;
    char *dirpath = "/proc";
    ssize_t res;

    if (argc < 2)
        return 1;

    char *file_path = argv[1];
    
    if ((info_dir = opendir(dirpath)) == NULL)
        errExit("opendir");

    while (dirent = readdir(info_dir)) {
        if ((strtol(dirent->d_name, &endptr, 0)) > 0 && !strlen(endptr)) {
            FILE *fp;
            char buffer[MAX_READ];
            char *PID = dirent->d_name;
            struct dirent *dirent_fd;
            DIR *info_dir_fd;

            size_t size_path_pid_fd = strlen(PID) + strlen(dirpath) + 5;
            
            char *path_pid_fd = malloc(size_path_pid_fd);
            if (path_pid_fd == NULL)
            errExit("malloc");
            
            snprintf(path_pid_fd, size_path_pid_fd, "%s/%s/fd", dirpath, PID);
            
            if ((info_dir_fd = opendir(path_pid_fd)) == NULL)
                continue;

            while ((dirent_fd = readdir(info_dir_fd)) == NULL) {
                char *fd = dirent_fd->d_name;
                printf("%s\n", fd);
                // res = readlink(path_pid_fd, buffer, MAX_READ);
                // printf("res: %zu, buffer: %s\n", );
            }



        }
    }


    return 0;
}