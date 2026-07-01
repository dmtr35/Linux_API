#define _GNU_SOURCE                     /* realpath */
#include <limits.h>                     /* realpath */
#include <stdlib.h>                     /* realpath */
#include <libgen.h>                     /* dirname, basename */
#include <stdio.h>                      /* printf */
#include <sys/stat.h>                   /* stat, lstat, fstat */
#include <sys/types.h>                  /* ino_t, dev_t, mode_t, uid_t, gid_t, off_t */
#include <time.h>                       /* ctime */
#include <string.h>                     /* strstr, strdup */
#include <dirent.h>                     /* opendir, readdir */

#include "../lib/error_functions.h"

#define MAX_PATH 256

typedef struct file_info {
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
} file_info;

typedef struct database {
    struct file_info *files;
    size_t count;
    size_t capacity;
} database;


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

void add_to_arr(char *path, struct stat *sb, database *db)
{
    if(db->capacity == db->count) {
        db->capacity *= 2;
        
        struct file_info *tmp = realloc(db->files, db->capacity * sizeof(struct file_info));

        if (tmp == NULL)
            errExit("realloc");

        db->files = tmp;
    }

    db->files[db->count].path = path;
    db->files[db->count].atime = sb->st_atim;
    db->files[db->count].ctime = sb->st_ctim;
    db->files[db->count].mtime = sb->st_mtim;
    db->files[db->count].inode = sb->st_ino;
    db->files[db->count].device = sb->st_dev;
    db->files[db->count].uid = sb->st_uid;
    db->files[db->count].gid = sb->st_gid;
    db->files[db->count].mode = sb->st_mode;
    db->files[db->count].size = sb->st_size;

    (db->count)++;
}

void func_tree(char *path, struct stat *sb, database *db)
{
    DIR *info_dir;
    struct dirent *dirent;
    // struct stat sb;

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
    char path[MAX_PATH];
    char path_file[MAX_PATH];
    char resolved_path[MAX_PATH];
    int flag_restor = 0, flag_info = 0;

    database db = {};
    db.count = 0;
    db.capacity = 2;
    db.files = malloc(db.capacity * sizeof(struct file_info));

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

    char *full_path = realpath(path, NULL);                 // выделяется память
    if (full_path == NULL)
        errExit("realpath");

    char *full_path_copy = strdup(full_path);               // выделяется память
    if (full_path_copy == NULL)
        errExit("strdup");

    char *parent_path = dirname(full_path_copy);            // обрезает full_path_copy (память не выделяется)


    if(stat(parent_path, &sb) == -1)
        errExit("stat");
    add_to_arr(parent_path, &sb, &db);
    
    if(stat(full_path, &sb) == -1)
        errExit("stat");
    
    

    if (S_ISDIR(sb.st_mode))
        func_tree(full_path, &sb, &db);
    else
        add_to_arr(full_path, &sb, &db);
        
                


                
    free(full_path);
    free(full_path_copy);
    return 0;
}