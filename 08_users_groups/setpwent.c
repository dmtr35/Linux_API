#include <pwd.h>                            /* getpwent, setpwent, endpwent */
#include <grp.h>                            /* getgrent, setgrent, endgrent */
// #include <stdio.h>
// #include <grp.h>
// #include <ctype.h>
// #include <stdlib.h>


// #include "../lib/ugid_functions.h"        /* Объявление определяемых здесь функций */
#include "../lib/tlpi_hdr.h"
#include "../lib/error_functions.h"

// Возвращает указатель при успешном завершении или NULL в случае конца потока или при ошибке
/* поочередно возвращает записи из файла паролей, выдавая NULL,
когда записей уже больше нет (или при возникновении ошибки) 
При первом вызове функция автоматически открывает файл паролей.*/
struct passwd *getpwent(void);

/* для перезапуска чтения с начала файла можно воспользоваться функцией setpwent() */
void setpwent(void);

// для его закрытия файла паролей вызывается функция endpwent()
void endpwent(void);
// -------------------------------------------------------------------

/* getgrent(), setgrent() и endgrent() выполняют аналогичные задачи для файла групп */
struct group *getgrent(void);
void setgrent(void);
void endgrent(void);

int main()
{
    struct passwd *pwd;

    while ((pwd = getpwent()) != NULL)
        printf("%-20s %5ld\n", pwd->pw_name, (long)pwd->pw_uid);

    endpwent();                     // закрыть файл паролей, чтобы при повторном вызове он начинал вывод сначала
    // ====================================================================
    puts("===============================================================");

    struct group *gr;
    while ((gr = getgrent()) != NULL)
        printf("%-20s %5ld\n", gr->gr_name, (long)gr->gr_gid);

    void endgrent(void);

    return 0;
}


