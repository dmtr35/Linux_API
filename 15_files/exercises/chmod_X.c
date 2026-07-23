#define _GNU_SOURCE                             /* realpath, PATH_MAX, S_IFMT, S_IFREG */
#include <limits.h>                             /* realpath, PATH_MAX */
#include <stdio.h>                              /* printf */
#include <dirent.h>                             /* opendir, readdir */
#include <sys/stat.h>                           /* umask, chmod, fchmod, S_IRWXU, S_IRWXG, S_IRWXO */
#include <stdlib.h>                             /* EXIT_SUCCESS, EXIT_FAILURE */
#include <fcntl.h>                              /* open, O_RDONLY, O_RDWR, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND */
#include <sys/stat.h>                           /* stat, lstat, fstat */
#include <string.h>                             /* strstr */
#include "../../lib/error_functions.h"


void chmod_file(struct stat *sb, char *pathname)
{
    mode_t mode;

    if (sb->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
        mode = (sb->st_mode | S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH);
    else
        mode = (sb->st_mode | S_IRUSR | S_IRGRP | S_IROTH);

    if (chmod(pathname, mode) == -1)
        errExit("chmod");
}

void chmod_dir_recur(struct stat *sb, char *dirpath, int flag_recur)
{
    DIR *info_dir;
    struct dirent *dirent;
    struct stat sb_new;
    mode_t mode;

    mode = (sb->st_mode | S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXGRP | S_IXOTH);
    if (chmod(dirpath, mode) == -1)
        errExit("chmod");

    if (!flag_recur)
        return;

    if ((info_dir = opendir(dirpath)) == NULL)
        errExit("opendir");

    while (dirent = readdir(info_dir)) {
        char *file_name = dirent->d_name;
        if (strcmp(file_name, "..") != 0 && strcmp(file_name, ".") != 0) {
            size_t size_path = strlen(file_name) + strlen(dirpath) + 2;
            char *new_path = malloc(size_path);
            snprintf(new_path, size_path, "%s/%s", dirpath, file_name);

            if (stat(new_path, &sb_new) == -1)
                errExit("stat");

            if (S_ISDIR(sb_new.st_mode)) {
                chmod_dir_recur(&sb_new, new_path, 1);
            } else {
                chmod_file(&sb_new, new_path);
            }            
        }
    }
}


int main(int argc, char *argv[])
{
    int fd, res, pos = 1, flag_recur = 0;
    char *file_name;
    struct stat sb;


    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-R] file_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strstr(argv[pos], "-R") != NULL) {
        flag_recur = 1;
        pos++;
    }

    
    while (file_name = argv[pos]) {
        char *path = realpath(file_name, NULL);

        if (path == NULL) {
            perror("realpath");
            exit(EXIT_FAILURE);
        }

        if (stat(path, &sb))
            errExit("fstat");

        if (S_ISDIR(sb.st_mode)) {
            chmod_dir_recur(&sb, path, flag_recur);
        } else {
            chmod_file(&sb, path);
        }
        
        printf("%s\n", argv[pos++]);
    }

    // pathname = argv[1];
    
    // fd = open(pathname, O_RDONLY);
    // if (fd == -1)
    //     errExit("open");
    

    
    


    return 0;
}
