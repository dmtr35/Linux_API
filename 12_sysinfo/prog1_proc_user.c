#include <stdio.h>                              /* printf, snprintf */
#include <dirent.h>                             /* opendir, readdir */
#include <stdlib.h>                             /* strtol */
#include <string.h>                             /* strncmp */
#include "../lib/ugid_functions.h"              /* userIdFromName */
#include "../lib/error_functions.h"             /* err */

//  Возвращает описатель потока каталога или NULL при ошибке
/*  Функция opendir() открывает каталог и возвращает описатель,
который можно применять для ссылки на каталог в последующих вызовах */
DIR *opendir(const char *dirpath);

//  Функция readdir() считывает последовательные записи из потока каталога.
/*  Возвращает указатель на статически выделенную структуру, описывающую
следующую запись каталога, или NULL при достижении конца каталога или при ошибке */
struct dirent *readdir(DIR *dirp);


/* 
    Программа принимает имя пользователя в качестве аргумента командной строки.
    С помощью userIdFromName() получает UID указанного пользователя,
    используя opendir() и readdir() обходит каталог /proc,
    анализирует строки Name: и Uid: в файлах /proc/PID/status и выводит список PID
    и имен команд процессов, принадлежащих этому пользователю
*/

#define MAX_READ 64

int main(int argc, char *argv[])
{
    uid_t user_uid;
    DIR *info_dir;
    struct dirent *dirent;
    char *endptr;
    char *dirpath = "/proc";

    if (argc < 2)
        return 1;
    char *user_name = argv[1];

    user_uid = userIdFromName(user_name);
    if (user_uid == -1)
        errExit("userIdFromName");

    if ((info_dir = opendir(dirpath)) == NULL)
        errExit("opendir");
    
    
    while(dirent = readdir(info_dir)) {
        if ((strtol(dirent->d_name, &endptr, 0)) > 0 && !strlen(endptr)) {
            FILE *fp;
            char buffer[MAX_READ];
            char *PID = dirent->d_name;
            size_t size_path_status = strlen(PID) + strlen(dirpath) + strlen("status") + 3;

            char *path_status = malloc(size_path_status);
            if (path_status == NULL)
                errExit("malloc");

            snprintf(path_status, size_path_status, "%s/%s/status", dirpath, PID);
            
            fp = fopen(path_status, "r");
            if (fp == NULL) {
                free(path_status);
                break;;
            }

            char name[MAX_READ];
            while(fgets(buffer, MAX_READ - 1, fp) != NULL) {
                if (strncmp(buffer, "Name:", 5) == 0) {
                    strcpy(name, buffer + 6);
                    name[strcspn(name, "\n")] = '\0';
                } else if (strncmp(buffer, "Uid:", 4) == 0) {
                    uid_t proc_uid;
                    sscanf(buffer, "Uid:\t%u", &proc_uid);

                    if (proc_uid == user_uid) {
                        printf("%s: %s\n", PID, name);
                        continue;
                    } else {
                        continue;
                    }
                }
            }
            free(path_status);
            fclose(fp);
        }
    }
    return 0;
}