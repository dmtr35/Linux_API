#define _GNU_SOURCE                                 /* getresuid, getresgid */
#include <unistd.h>                                 /* access, getresuid, getresgid, R_OK, W_OK, X_OK, F_OK */
#include <sys/stat.h>                               /* stat, struct stat, mode_t, S_IRUSR, S_IWUSR, S_IXUSR */
#include <sys/types.h>                              /* uid_t, gid_t */
#include <stdio.h>                                  /* printf, fprintf */
#include <limits.h>                                 /* NGROUPS_MAX */
#include <stdlib.h>                                 /* EXIT_SUCCESS, EXIT_FAILURE */
#include "../../lib/error_functions.h"
#include "../../lib/file_perms.h"


// int access(const char *pathname, int mode);
/*  Константы mode для системного вызова access()
    Константа           Описание
    F_OK                Существует ли файл?
    R_OK                Можно ли читать файл?
    W_OK                Можно ли записывать файл?
    X_OK                Можно ли выполнять файл?    */
    
// int fchmod(int fd, mode_t mode);


int check_gr(const struct stat *sb)
{
    int count_gr;
    gid_t grouplist[NGROUPS_MAX + 1];

    count_gr = getgroups(NGROUPS_MAX, grouplist);
    if (count_gr == -1)
        return 0;

    for (int i = 0; i < count_gr; ++i)
        if (sb->st_gid == grouplist[i])
            return 1;

    return 0;
}

void check_perm(const struct stat *sb, int *result, uid_t euid, gid_t egid, int USR, int mode)
{
    uid_t uid_file = sb->st_uid;
    gid_t gid_file = sb->st_gid;
    mode_t perm = sb->st_mode;
    int GRP = USR >> 3;
    int OTH = USR >> 6;

    if (uid_file == euid) {
        if (perm & USR)
            *result |= mode;
    } else if (gid_file == egid || check_gr(sb)) {
        if (perm & GRP)
            *result |= mode;
    } else {
        if (perm & OTH)
            *result |= mode;
    }

}

int my_access(const char *pathname, int mode)
{
    int result = 0;
    struct stat sb;

    if (stat(pathname, &sb) == -1)
        return -1;

    if (mode == F_OK)
        return 0;

    mode_t perm = sb.st_mode;
    uid_t ruid, euid, suid;
    gid_t rgid, egid, sgid;
    
    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");

    if (getresgid(&rgid, &egid, &sgid) == -1)
        errExit("getresgid");

    if (mode & R_OK) {
        if (euid == 0)
            result |= R_OK;
        else
            check_perm(&sb, &result, euid, egid, S_IRUSR, R_OK);
    }
    if (mode & W_OK) {
        if (euid == 0)
            result |= W_OK;
        else
            check_perm(&sb, &result, euid, egid, S_IWUSR, W_OK);
    }
    
    if (mode & X_OK) {
        if (euid == 0) {
            if (perm & (S_IXUSR | S_IXGRP | S_IXOTH))
            result |= X_OK;
        } else {
            check_perm(&sb, &result, euid, egid, S_IXUSR, X_OK);
        }
    }

    if (result == mode)
        return 0;
    else
        return -1;

}


int main(int argc, char *argv[])
{
    char *pathname = argv[1];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s pathname\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    
    if (my_access(pathname, F_OK) == 0)
        printf("file %s существует\n", pathname);

    if (my_access(pathname, R_OK) == 0)
        printf("file %s читаемый\n", pathname);

    if (my_access(pathname, W_OK) == 0)
        printf("file %s записываемый\n", pathname);

    if (my_access(pathname, X_OK) == 0)
        printf("file %s исполняемый\n", pathname);

    if (my_access(pathname, R_OK | X_OK) == 0)
        printf("file %s читаем, и исполняемый\n", pathname);
    
    if (my_access(pathname, R_OK | W_OK) == 0)
        printf("file %s читаем, записываемый\n", pathname);

    if (my_access(pathname, W_OK | X_OK) == 0)
        printf("file %s записываемый и исполняемый\n", pathname);

    if (my_access(pathname, R_OK | W_OK | X_OK) == 0)
        printf("file %s читаем, записываемый и исполняемый\n", pathname);

    return 0;
}
