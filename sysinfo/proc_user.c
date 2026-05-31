#include <stdio.h>                      /* printf */
#include <dirent.h>                     /* opendir, readdir */
#include "../lib/ugid_functions.h"      /* userIdFromName */

//  Возвращает описатель потока каталога или NULL при ошибке
/*  Функция opendir() открывает каталог и возвращает описатель,
который можно применять для ссылки на каталог в последующих вызовах */
DIR *opendir(const char *dirpath);

//  Функция readdir() считывает последовательные записи из потока каталога.
/*  Возвращает указатель на статически выделенную структуру, описывающую
следующую запись каталога, или NULL при достижении конца каталога или при ошибке */
struct dirent *readdir(DIR *dirp);

int main(int argc, char *argv[])
{
    char *user_name = argv[1];
    uid_t user_uid;
    char *dirpath = "/proc";
    DIR *info_dir;
    struct dirent dir;

    user_uid = userIdFromName(user_name);
    dir = readdir();
    info_dir = opendir(dirpath);

    printf("%d\n", user_uid);

    return 0;
}