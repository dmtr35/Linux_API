#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>                                     /* EXIT_SUCCESS */
#include <string.h>                                     /* strcmp */
#include "../lib/error_functions.h"
#include "../lib/file_perms.h"


// #include "file_perms.h"/* Интерфейс для данной реализации */

#define STR_SIZE sizeof("rwxrwxrwx")

/*  Константы для битов прав доступа к файлу
    Константа       Восьмеричное значение       Бит прав доступа
    S_ISUID         04000                       Set-user-ID
    S_ISGID         02000                       Set-group-ID
    S_ISVTX         01000                       Закрепляющий
    S_IRUSR         0400                        Пользователь: чтение
    S_IWUSR         0200                        Пользователь: запись
    S_IXUSR         0100                        Пользователь: выполнение
    S_IRGRP         040                         Группа: чтение
    S_IWGRP         020                         Группа: запись
    S_IXGRP         010                         Группа: выполнение
    S_IROTH         04                          Остальные: чтение
    S_IWOTH         02                          Остальные: запись
    S_IXOTH         01                          Остальные: выполнение

    также определены три константы, чтобы уравнять маски всех трех прав доступа для каждой категории:
    S_IRWXU         0700                        владельца
    S_IRWXG         070                         группы
    S_IRWXO         07                          остальных
*/

/* Возвращает вместо маски прав доступа к файлу строку в стиле ls(1) */
char *filePermStr(mode_t perm, int flags)
{
    static char str[STR_SIZE];

    snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
        (perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
        (perm & S_IXUSR) ?
            (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
            (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
        (perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
        (perm & S_IXGRP) ?
            (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
            (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
        (perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
        (perm & S_IXOTH) ?
            (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
            (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-'));

    return str;
}