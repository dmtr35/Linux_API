#define _GNU_SOURCE                         /* getresuid, getresgid */
#include <unistd.h>                         /* access, getresuid, getresgid */
#include <sys/stat.h>                       /* umask, chmod, fchmod, S_IRWXU, S_IRWXG, S_IRWXO */
#include <sys/types.h>                      /* mode_t */
#include <stdio.h>                          /* printf */
#include <fcntl.h>                          /* open */
#include <libgen.h>                         /* dirname, basename */
#include <limits.h>                         /* NGROUPS_MAX */
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




int check_access(char *pathname)
{
    struct stat sb;

    if (stat(pathname, &sb) == -1)
        errExit("stat");

    if (access(pathname, F_OK) == -1) {
        printf("file %s НЕ существует\n", pathname);
    } else {
        printf("file %s существует\n", pathname);
        if (access(pathname, R_OK) == 0)
            printf("file %s читаемый\n", pathname);
        if (access(pathname, W_OK) == 0)
            printf("file %s записываемый\n", pathname);
        if (access(pathname, X_OK) == 0)
            printf("file %s можно выполнять\n", pathname);
    }
    printf("%s\n", filePermStr(sb.st_mode, 0));
    puts("================================================");
}


int check_gr(const struct stat *sb)
{
    int count_gr = 0;
    gid_t grouplist[NGROUPS_MAX + 1];

    count_gr = getgroups(NGROUPS_MAX, grouplist);
    if (count_gr == -1)
        return 0;

    for (int i = 0; i < count_gr; ++i) {
        if (sb->st_gid == grouplist[i]) {
            return 1;
        }
    }
    return 0;
}

int check_perm(const char *pathname, struct stat *sb, int mode)
{
    int USR, GRP, OTH;
    mode_t perm = sb->st_mode;
    uid_t ruid, euid, suid;
    gid_t rgid, egid, sgid;

    uid_t uid_file = sb->st_uid & 0777;
    gid_t gid_file = sb->st_gid & 0777;

    if (mode == R_OK) {
        USR = S_IRUSR;
        GRP = S_IRGRP;
        OTH = S_IROTH;
    } else if (mode == W_OK) {
        USR = S_IWUSR;
        GRP = S_IWGRP;
        OTH = S_IWOTH;
    } else if (mode == X_OK) {
        USR = S_IXUSR;
        GRP = S_IXGRP;
        OTH = S_IXOTH;
    }

    getresuid(&ruid, &euid, &suid);
    getresgid(&rgid, &egid, &sgid);

    if (uid_file == euid) {
        if (perm & USR) {
            printf("file %s читаемый\n", pathname);
            return 0;
        } else {
            return -1;
        }
    } else if (gid_file == egid || check_gr(sb)) {
        if (perm & GRP) {
            printf("file %s читаемый\n", pathname);
            return 0;
        } else {
            return -1;
        }
    } else {
        if (perm & OTH) {
            printf("file %s читаемый\n", pathname);
            return 0;
        } else {
            return -1;
        }
    }
}
// реализовать:
// Настоящий access() имеет особое поведение для root.
// Если:
// uid == 0
// то:
// R_OK и W_OK обычно разрешены
// X_OK требует хотя бы одного execute-бита

int my_access(const char *pathname, int mode)
{
    int res, flag_gr = 0, count_gr = 0;
    struct stat sb;

    res = stat(pathname, &sb);
    if (res == -1)
        return -1;

    if (mode == F_OK) {
        if (res == -1) {
            printf("file %s НЕ существует\n", pathname);
            return -1;
        }
    }

    printf("file %s существует\n", pathname);
    // printf("sb.st_uid: %d\n", sb.st_uid);
    // printf("sb.st_gid: %d\n", sb.st_gid);
    
    // res = setregid(-1, 1000);
    // res = setreuid(-1, 1000);

    // getresuid(&ruid, &euid, &suid);
    // getresgid(&rgid, &egid, &sgid);
    // printf("ruid: %d, euid: %d, suid: %d\n", ruid, euid, suid);
    // printf("rgid: %d, egid: %d, sgid: %d\n", rgid, egid, sgid);
    
    // printf("sb.st_uid: %d\n", sb.st_uid);
    // printf("sb.st_gid: %d\n", sb.st_gid);

    check_perm(pathname, &sb, R_OK);
    check_perm(pathname, &sb, W_OK);
    check_perm(pathname, &sb, X_OK);
}


int main(int argc, char *argv[])
{
    int fd, res;
    struct stat sb;
    mode_t mode;
    
    char *pathname = argv[1];

    // res = my_access(pathname, F_OK);
    res = my_access(pathname, R_OK);
    // res = my_access(pathname, W_OK);
    // res = my_access(pathname, X_OK);

    // if (access(pathname, F_OK) == 0)
    //     if (unlink(pathname) == -1)
    //         errMsg("unlink-%s", pathname);

    // fd = open(pathname, O_RDWR | O_CREAT, 0000);
    // check_access(pathname);
    
    // mode = (sb.st_mode | S_IRGRP | S_IWGRP | S_IXGRP);
    // fchmod(fd, mode);
    // check_access(pathname);
    // mode = (sb.st_mode | S_IRUSR | S_IWUSR | S_IXUSR & ~S_IRGRP & ~S_IWGRP & ~S_IXGRP);
    // fchmod(fd, mode);
    // check_access(pathname);


    // close(fd);
    return 0;
}