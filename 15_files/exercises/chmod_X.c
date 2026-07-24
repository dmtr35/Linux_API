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
#include "../../lib/file_perms.h"               /* filePermStr */



void make_mode(struct stat *sb, char *pathname, int flag_recur)
{
    DIR *info_dir;
    struct dirent *dirent;

    mode_t mode = (sb->st_mode | S_IRUSR | S_IRGRP | S_IROTH);
    if (S_ISDIR(sb->st_mode) || sb->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
        mode |= S_IXUSR | S_IXGRP | S_IXOTH;
    }

    if (chmod(pathname, mode) == -1)
        errExit("chmod");
    
    char *base = basename(pathname);
    printf("%-20s: %s > ", base, filePermStr(sb->st_mode, 0));
    printf("%s\n", filePermStr(mode, 0));

    if (!flag_recur)
        return;

    if ((info_dir = opendir(pathname)) == NULL)
        errExit("opendir");

    while ((dirent = readdir(info_dir)) != 0) {
        char *file_name = dirent->d_name;
        // if (strcmp(file_name, "..") && strcmp(file_name, ".")) {
        if (strcmp(file_name, "..") != 0 && strcmp(file_name, ".") != 0) {
            struct stat sb_new;
            size_t size_path = strlen(file_name) + strlen(pathname) + 2;
            char *new_path = malloc(size_path);
            if (new_path == NULL)
                errExit("malloc");

            snprintf(new_path, size_path, "%s/%s", pathname, file_name);

            if (stat(new_path, &sb_new) == -1)
                errExit("stat");

            make_mode(&sb_new, new_path, S_ISDIR(sb_new.st_mode) ? 1 : 0);

            free(new_path);          
        }
    }

    closedir(info_dir);
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

    if (strcmp(argv[pos], "-R") == 0) {
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
            errExit("stat");

        make_mode(&sb, path, flag_recur);
        // if (S_ISDIR(sb.st_mode)) {
            // chmod_dir_recur(&sb, path, flag_recur);
        // } else {
            // chmod_file(&sb, path);
        // }
        
        free(path);
        pos++;
    }

    return 0;
}
