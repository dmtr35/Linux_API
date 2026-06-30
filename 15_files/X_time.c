#define _GNU_SOURCE                     /* realpath */
#include <stdio.h>                      /* printf */
#include <sys/stat.h>                   /* stat, lstat, fstat */
#include <sys/types.h>                  /* ino_t, dev_t, mode_t, uid_t, gid_t, off_t */
#include <time.h>                       /* ctime */
#include <string.h>                     /* strstr, strdup */
#include <libgen.h>                     /* dirname, basename */
#include <limits.h>                     /* realpath */
#include <stdlib.h>                     /* realpath */
#include <dirent.h>                     /* opendir, readdir */

#include "../lib/error_functions.h"

#define MAX_PATH 256

struct file_info {
    ino_t inode;
    dev_t device;
    mode_t mode;
    uid_t uid;
    gid_t gid;
    off_t size;
    struct timespec atime;
    struct timespec mtime;
    struct timespec ctime;
    char *path;
};

struct database {
    struct file_info *files;
    size_t count;
    size_t capacity;
};


void print_info(char *text, char *path, struct stat *sb)
{
    printf("%s:\n", text);
    printf("%s:\n", path);
    printf("I-node number:              %ld\n", (long) sb->st_ino);
    // printf("Mode:               %lo (%s)\n", (unsigned long)st->st_mode, filePermStr(st->st_mode, 0));
    printf("Last file access:           %s", ctime(&sb->st_atime));
    printf("Last file modification:     %s", ctime(&sb->st_mtime));
    printf("Last status change:         %s", ctime(&sb->st_ctime));            
}

void add_to_arr(char *path, struct stat *sb)
{

}

void func_tree(char *path)
{
    DIR *info_dir;
    struct dirent *dirent;
    struct stat sb;

    if ((info_dir = opendir(path)) == NULL)
        errExit("opendir");
    
    
    while(dirent = readdir(info_dir)) {
        printf("f");
    }
}
    
int main(int argc, char *argv[])
{
    int res;
    struct stat sb, sb_perent;
    // DIR *info_dir;
    // struct dirent *dirent;
    char path[MAX_PATH];
    char path_file[MAX_PATH];
    char resolved_path[MAX_PATH];
    int flag_restor = 0, flag_info = 0;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageErr("%s file [-r] path [path]\n", argv[0]);
    } else if (argc == 2) {
        flag_info = 1;
        strcpy(path, argv[1]);
    } else if (argc == 3) {
        strcpy(path, argv[1]);
        strcpy(path_file, argv[2]);
    } else if (argc == 4 && strstr(argv[1], "-r") != NULL) {
        flag_restor = 1;
        strcpy(path, argv[2]);
        strcpy(path_file, argv[3]);
    }

    char *full_path = realpath(path, NULL);
    if (full_path == NULL)
        errExit("realpath");

    char *parent_path= strdup(full_path);
    if(parent_path== NULL)
        errExit("strdup");

    dirname(parent_path);



    if(stat(full_path, &sb) == -1)
        errExit("stat");
    if(stat(parent_path, &sb_perent) == -1)
        errExit("stat");

    if (S_ISDIR(sb.st_mode)) {
        func_tree(full_path);

    } else {
        printf("g");
    }

    // if(flag_info) {
    //     print_info("dir_parent", parent_path, &sb_perent);
    //     printf("------------------------------------------------------\n");
    //     print_info("file", full_path, &sb);
    //     return 0;
    // }


    // check_Xtime(&sb);



    return 0;
}