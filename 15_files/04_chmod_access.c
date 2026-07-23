#include <unistd.h>                         /* access */
#include <sys/stat.h>                       /* umask, chmod, fchmod, S_IRWXU, S_IRWXG, S_IRWXO */
#include <sys/types.h>                      /* mode_t */
#include <stdio.h>                          /* printf */
#include <fcntl.h>                          /* open */
#include "../lib/error_functions.h"
#include "../lib/file_perms.h"

/*  вызов access() проверяет доступность файла, указанного в аргументе
    pathname, на основе реальных идентификаторов пользователя и группы
    (а также идентификаторов добавочных групп) для процесса. */

//  Возвращает 0, если предоставлены все права доступа, и -1 в противном случае
//  следует избегеть, есть уязвимость гонки!!
int access(const char *pathname, int mode);
/*  Константы mode для системного вызова access()
    Константа           Описание
    F_OK                Существует ли файл?
    R_OK                Можно ли читать файл?
    W_OK                Можно ли записывать файл?
    X_OK                Можно ли выполнять файл?    */
//  ==================================================

//  Всегда успешно возвращает параметр umask предыдущего процесса
mode_t umask(mode_t mask);

//  ==================================================

//  Оба вызова возвращают 0 при успешном завершении и -1 при ошибке
int chmod(const char *pathname, mode_t mode);
int fchmod(int fd, mode_t mode);


int main(int argc, char *argv[])
{
    char *pathname = "startup";
    int res, fd;
    mode_t u, mode;;
    struct stat sb;

    // res = access("15_files", F_OK);
    res = access(pathname, R_OK);
    res = access(pathname, W_OK);
    res = access(pathname, X_OK);
//  ==================================================

    u = umask(0);
    printf("perm: %s\n", filePermStr(u, 0));

    if (access(pathname, F_OK) == 0)
        if (unlink(pathname) == -1)
            errMsg("unlink-%s", pathname);

    fd = open(pathname, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

    if (stat(pathname, &sb) == -1)
        errExit("stat");
    printf("perm: %s\n", filePermStr(sb.st_mode, 0));
    
    mode = (sb.st_mode | S_IWUSR) & ~S_IROTH;       /* Разрешить владельцу запись, другим пользователям запретить чтение,остальные биты не менять */
    printf("perm: %s\n", filePermStr(mode, 0));
    
    if (fchmod(fd, mode) == -1)
        errExit("chmod");
    
    if (stat("startup", &sb) == -1)
        errExit("stat");
    printf("perm: %s\n", filePermStr(sb.st_mode, 0));

    umask(u);

    return 0;
}