#define _GNU_SOURCE             /* settimeofday */
#include <locale.h>             /* setlocale */
#include <stdio.h>              /* printf */
#include <sys/time.h>           /* settimeofday */


/*
    Информации о локали содержится в иерархии каталогов, которая находится в каталоге /usr/share/locale
    (или в некоторых дистрибутивах в каталоге /usr/lib/locale)

    Команда locale выводит информацию о текущей локали среды (в оболочке).
    Команда locale -a выводит списком полный набор локалей, определенных в системе.
*/

//  Задание для программы локали
/*  Возвращает указатель на (обычно статически выделенную) строку, определяющую новые
    или текущие местные настройки, при успехе или NULL при ошибке */
char *setlocale(int category, const char *locale);

// --------------------------------------------------------------

// Обновление системных часов
// Возвращает при успешном завершении 0 или -1 при ошибке
int settimeofday(const struct timeval *tv, const struct timezone *tz);

// --------------------------------------------------------------

// При внесении незначительных изменений в установки времени (в пределах нескольких секунд)
// Возвращает при успешном завершении 0 или -1 при ошибке
int adjtime(struct timeval *delta, struct timeval *olddelta);


int main()
{
    char *locale;

    locale = setlocale(LC_ALL, "C");
    printf("locale: %s\n",locale);


    return 0;
}


/*
    LC_CTYPE=de_DES.UTF-8;
    LC_NUMERIC=de_DES.UTF-8;
    LC_TIME=de_DES.UTF-8;
    LC_COLLATE=de_DES.UTF-8;
    LC_MONETARY=de_DES.UTF-8;
    LC_MESSAGES=de_DES.UTF-8;
    LC_PAPER=de_DES.UTF-8;
    LC_NAME=de_DES.UTF-8;
    LC_ADDRESS=de_DES.UTF-8;
    LC_TELEPHONE=de_DES.UTF-8;
    LC_MEASUREMENT=de_DES.UTF-8;
    LC_IDENTIFICATION=de_DES.UTF-8
*/
// LC_CTYPE=de_DES.UTF-8 LC_NUMERIC=de_DES.UTF-8 LC_TIME=de_DES.UTF-8 LC_COLLATE=de_DES.UTF-8 LC_MONETARY=de_DES.UTF-8 LC_MESSAGES=de_DES.UTF-8 LC_PAPER=de_DES.UTF-8 LC_NAME=de_DES.UTF-8 LC_ADDRESS=de_DES.UTF-8 LC_TELEPHONE=de_DES.UTF-8 LC_MEASUREMENT=de_DES.UTF-8 LC_IDENTIFICATION=de_DES.UTF-8