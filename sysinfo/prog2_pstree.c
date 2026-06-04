#include <stdio.h>                      /* printf, snprintf */
#include <dirent.h>                     /* opendir, readdir */
#include <stdlib.h>                     /* strtol */
#include <string.h>                     /* strncmp */
#include "../lib/tlpi_hdr.h"
#include "../lib/error_functions.h"     /* err */

#define MAX_READ 64

typedef struct proc_info {
    pid_t pid;
    pid_t ppid;
    char name[MAX_READ];
} proc_info;

void print_indent(int level)
{
    for (int i = 0; i < level; i++)
        printf("\t");
}

void printf_tree(int depth, pid_t p, proc_info list[], int length, int flag)
{
    if (flag == 0) {
        for (int i = 0; i < length; ++i) {
            if (p == list[i].pid) {
                printf("%s(%d)\n", list[i].name, list[i].pid);
                break;
            }
        }
    }

    for (int i = 0; i < length; ++i) {
        if (p == list[i].ppid) {
            print_indent(depth);
            printf("└─%s(%d)\n", list[i].name, list[i].pid);
            printf_tree(depth + 1, list[i].pid, list, length, 1);
        }
    }
}
int main(int argc, char *argv[])
{
    proc_info *list = NULL;
    size_t cap = 0;
    size_t len = 0;
    int res, flag = 0;
    pid_t p;
    
    char *dirpath = "/proc";
    DIR *info_dir;
    struct dirent *dirent;
    char *endptr;
    p = argc > 1 ? strtol(argv[1], &endptr, 0) : 1;

    if ((info_dir = opendir(dirpath)) == NULL)
        errExit("opendir");
    
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
                continue;
            }

            pid_t pid_tmp = strtol(PID, &endptr, 0);
            pid_t ppid_tmp = -1;
            char name[MAX_READ] = "";

            while(fgets(buffer, MAX_READ - 1, fp) != NULL) {
                if (strncmp(buffer, "Name:", 5) == 0) {
                    strcpy(name, buffer + 6);
                    name[strcspn(name, "\n")] = '\0';
                } else if (strncmp(buffer, "PPid:", 5) == 0) {
                    ppid_tmp = strtol(buffer + 6, &endptr, 0);
                    break;
                }
            }

            strcpy(list[len].name, name);
            list[len].pid = pid_tmp;
            list[len++].ppid = ppid_tmp;

            free(path_status);
            fclose(fp);
        }
    }
    
    closedir(info_dir);
    
    printf_tree(1, p, list, len, flag);
    free(list);


    return 0;
}