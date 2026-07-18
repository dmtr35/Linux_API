#include <unistd.h>                         /* access */
#include <sys/stat.h>                       /* umask, chmod, fchmod, S_IRWXU, S_IRWXG, S_IRWXO */
#include <sys/types.h>                      /* mode_t */
#include <stdio.h>                          /* printf */
#include <fcntl.h>                          /* open */
#include <libgen.h>                         /* dirname, basename */
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

int main(int argc, char *argv[])
{
    int fd, res;
    struct stat sb;
    mode_t mode;
    
    char *pathname = argv[1];

    if (access(pathname, F_OK) == 0)
        if (unlink(pathname) == -1)
            errMsg("unlink-%s", pathname);

    fd = open(pathname, O_RDWR | O_CREAT, 0000);
    check_access(pathname);
    
    mode = (sb.st_mode | S_IRGRP | S_IWGRP | S_IXGRP);
    fchmod(fd, mode);
    check_access(pathname);
    mode = (sb.st_mode | S_IRUSR | S_IWUSR | S_IXUSR & ~S_IRGRP & ~S_IWGRP & ~S_IXGRP);
    fchmod(fd, mode);
    check_access(pathname);


    close(fd);
    return 0;
}