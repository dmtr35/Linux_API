#include <stdio.h>                      /* printf, snprintf */
#include <dirent.h>                     /* opendir, readdir */
#include <stdlib.h>                     /* strtol */
#include <string.h>                     /* strlen */
#include "../lib/ugid_functions.h"      /* userIdFromName */
#include "../lib/error_functions.h"      /* err */

//  Возвращает описатель потока каталога или NULL при ошибке
/*  Функция opendir() открывает каталог и возвращает описатель,
который можно применять для ссылки на каталог в последующих вызовах */
DIR *opendir(const char *dirpath);

//  Функция readdir() считывает последовательные записи из потока каталога.
/*  Возвращает указатель на статически выделенную структуру, описывающую
следующую запись каталога, или NULL при достижении конца каталога или при ошибке */
struct dirent *readdir(DIR *dirp);

#define MAX_READ 64
#define MAX_NAME 64


int main(int argc, char *argv[])
{
    int PID;
    uid_t user_uid;
    DIR *info_dir;
    struct dirent *dirent;
    char *endptr;
    char *user_name = argv[1];
    char *dirpath = "/proc";
    char bf4[10];

    user_uid = userIdFromName(user_name);
    if (user_uid == -1)
        errExit("userIdFromName");

    if ((info_dir = opendir(dirpath)) == NULL)
        errExit("opendir");
    
    
    while(dirent = readdir(info_dir)) {
        if ((PID = strtol(dirent->d_name, &endptr, 0)) > 0 && !strlen(endptr)) {
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
              errExit("fopen");
            }

            char name[MAX_READ];
            char uid[MAX_READ];
            while (fgets(buffer, MAX_READ - 1, fp) != NULL) {

                if (strstr(buffer, "Name") != NULL) {
                    strcpy(name, buffer + 6);
                } else if (strstr(buffer, "Uid") != NULL) {
                    strcpy(uid, buffer + 5);
                    char *end = strchr(uid, '\t');
                    end[0] = '\0';
                }
                if (strtol(uid, &endptr, 0) == user_uid)
                    printf("%s: %s", PID, name);
            }    
            
        }
    }
    

    return 0;
}