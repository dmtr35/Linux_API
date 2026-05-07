#include <pwd.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// Извлечение записей из файла паролей
// при успешном завершении возвращают указатель, при ошибке — NULL
struct passwd *getpwnam(const char *name);
struct passwd *getpwuid(uid_t uid);


int main()
{
    struct passwd *pwd_getpwnam;
    
    errno = 0;
    pwd_getpwnam = getpwnam("dm");
    if (pwd_getpwnam == NULL) {
        if (errno == 0)
            puts("Запись не найдена");
        else
            puts("Ошибка");
        }

    
    printf(
        "%-10s: %s\n"
        "%-10s: %s\n"
        "%-10s: %d\n"
        "%-10s: %d\n"
        "%-10s: %s\n"
        "%-10s: %s\n"
        "%-10s: %s\n",
        "pw_name",   pwd_getpwnam->pw_name,                 /* Имя для входа в систему (имя пользователя) */
        "pw_passwd", pwd_getpwnam->pw_passwd,               /* Зашифрованный пароль */
        "pw_uid",    pwd_getpwnam->pw_uid,                  /* Идентификатор пользователя */
        "pw_gid",    pwd_getpwnam->pw_gid,                  /* Идентификатор группы */
        "pw_gecos",  pwd_getpwnam->pw_gecos,                /* Комментарий (информация о пользователе) */
        "pw_dir",    pwd_getpwnam->pw_dir,                  /* Исходный рабочий (домашний) каталог */
        "pw_shell",  pwd_getpwnam->pw_shell                 /* Оболочка входа в систему */
    );
    
    puts("====================================================");
    struct passwd *pwd_getpwuid;

    errno = 0;
    pwd_getpwuid = getpwuid(0);
    
    pwd_getpwuid = getpwnam("dm");
    if (pwd_getpwuid == NULL) {
        if (errno == 0)
            puts("Запись не найдена");
        else
            puts("Ошибка");
        }

    printf(
        "%-10s: %s\n"
        "%-10s: %s\n"
        "%-10s: %d\n"
        "%-10s: %d\n"
        "%-10s: %s\n"
        "%-10s: %s\n"
        "%-10s: %s\n",
        "pw_name",   pwd_getpwuid->pw_name,                 /* Имя для входа в систему (имя пользователя) */
        "pw_passwd", pwd_getpwuid->pw_passwd,               /* Зашифрованный пароль */
        "pw_uid",    pwd_getpwuid->pw_uid,                  /* Идентификатор пользователя */
        "pw_gid",    pwd_getpwuid->pw_gid,                  /* Идентификатор группы */
        "pw_gecos",  pwd_getpwuid->pw_gecos,                /* Комментарий (информация о пользователе) */
        "pw_dir",    pwd_getpwuid->pw_dir,                  /* Исходный рабочий (домашний) каталог */
        "pw_shell",  pwd_getpwuid->pw_shell                 /* Оболочка входа в систему */
    );
    // ==================================================================











    return 0;
}