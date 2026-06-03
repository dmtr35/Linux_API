#include <stdio.h>                      /* printf, snprintf */
#include <dirent.h>                     /* opendir, readdir */
#include <stdlib.h>                     /* strtol */
#include <string.h>                     /* strncmp */
#include "../lib/ugid_functions.h"      /* userIdFromName */
#include "../lib/error_functions.h"      /* err */

#define MAX_READ 64

typedef struct proc_info {
    pid_t pid;
    pid_t ppid;
    char name[56];
} proc_info;

int main(int argc, char *argv[])
{
    proc_info *list = NULL;
    size_t cap = 0;
    size_t len = 0;

    char *dirpath = "/proc";
    DIR *info_dir;
    struct dirent *dirent;
    char *endptr;
    uid_t user_uid;

    if ((info_dir = opendir(dirpath)) == NULL)
        errExit("opendir");
    
    int i = 0;
    while(dirent = readdir(info_dir)) {
        if ((strtol(dirent->d_name, &endptr, 0)) > 0 && !strlen(endptr)) {
            if (cap == len) {
                cap = cap ? cap * 2 : 2;

                proc_info *tmp = realloc(list, cap * sizeof(proc_info));
                if (tmp == NULL)
                    errExit("realloc");
                list = tmp;
            }

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
                continue;;;
            }

            size_t pid = strtol(PID, &endptr, 0);
            size_t ppid;
            char name[MAX_READ];

            while(fgets(buffer, MAX_READ - 1, fp) != NULL) {
                if (strncmp(buffer, "Name:", 5) == 0) {
                    strcpy(name, buffer + 6);
                    name[strcspn(name, "\n")] = '\0';
                } else if (strncmp(buffer, "PPid:", 5) == 0) {
                    ppid = strtol(buffer + 6, &endptr, 0);
                    break;
                }
            }

            strncpy(list[len].name, name, strlen(name));
            list[len].pid = pid;
            list[len++].ppid = ppid;

            free(path_status);
            fclose(fp);
        }
    }
    return 0;
}