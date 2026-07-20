#include <pwd.h>            /* getpwnam, getpwuid */
#include <grp.h>            /* getgrnam, getgrgid */
#include "../lib/tlpi_hdr.h"
#include "../lib/error_functions.h"

//  Извлечение записей из файла паролей
/*  при успешном завершении возвращают указатель,
    при ошибке — NULL (не изменяя errno).
    если NULL + errno то произошла ошибка   */
struct passwd *getpwnam(const char *name);      // поиск по имени пользователя
struct passwd *getpwuid(uid_t uid);             // поиск по идентификатору пользователя
// -----------------------------------------------------------

//  Извлечение записей из файла групп
/*  при успешном завершении возвращают указатель,
    при ошибке — NULL   */
struct group *getgrnam(const char *name);       // поиск по имени группы
struct group *getgrgid(gid_t gid);              // поиск по идентификатору группы


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
    
    puts("===========================================================");
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
    puts("===========================================================");
    // ==================================================================
    
    struct group *gr_getgrnam;
    
    gr_getgrnam = getgrnam("sudo");
    
    printf(
        "%-10s: %s\n"
        "%-10s: %s\n"
        "%-10s: %d\n",
        "gr_name", gr_getgrnam->gr_name,        /* Имя группы */
        "gr_passwd", gr_getgrnam->gr_passwd,    /* Зашифрованный пароль (в режиме без теневых паролей) */
        "gr_gid", gr_getgrnam->gr_gid           /* Идентификатор группы */
    );
    
    /* Массив указателей на имена участников группы, перечисленных в /etc/group, завершающийся значением NULL */
    printf("%-10s: ", "gr_mem");
    for (char **mem = gr_getgrnam->gr_mem; *mem != NULL; mem++)
        printf("%s ", *mem);
    printf("\n");
    puts("===========================================================");
    // ==================================================================
    
    struct group *gr_getgrgid;
    
    gr_getgrgid = getgrgid(1000);

    printf(
        "%-10s: %s\n"
        "%-10s: %s\n"
        "%-10s: %d\n",
        "gr_name", gr_getgrgid->gr_name,        /* Имя группы */
        "gr_passwd", gr_getgrgid->gr_passwd,    /* Зашифрованный пароль (в режиме без теневых паролей) */
        "gr_gid", gr_getgrgid->gr_gid           /* Идентификатор группы */
    );
    
    /* Массив указателей на имена участников группы, перечисленных в /etc/group, завершающийся значением NULL */
    printf("%-10s: ", "gr_mem");
    for (char **mem = gr_getgrgid->gr_mem; *mem != NULL; mem++)
        printf("%s ", *mem);
    printf("\n");
    
    puts("===========================================================");
    // ==================================================================

    return 0;
}