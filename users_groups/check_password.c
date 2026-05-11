#define _GNU_SOURCE
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{
    char *username, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;
    Boolean authOk;
    size_t len;
    long lnmax;

    lnmax = sysconf(_SC_LOGIN_NAME_MAX);                    /* выдает максимальный размер имени пользователя в главной системе. */
    if (lnmax == -1)                                        /* Если предел не определен, */
        lnmax = 256;                                        /* выбираем наугад */

    username = malloc(lnmax);
    if (username == NULL)
        errExit("malloc");
    printf("Username: ");
    fflush(stdout);                                         /* заставляет немедленно вытолкнуть буфер stdout в терминал/файл */
    if (fgets(username, lnmax, stdin) == NULL)
        exit(EXIT_FAILURE);                                 /* Выход при встрече EOF */

    len = strlen(username);
    if (username[len - 1] == '\n')
        username[len - 1] = '\0';                           /* Удаление завершающего '\n' */

    pwd = getpwnam(username);
    if (pwd == NULL)
        fatal("couldn't get password record");
    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES)
        fatal("no permission to read shadow password file");

    if (spwd != NULL)                                       /* Если есть запись теневого пароля */
        pwd->pw_passwd = spwd->sp_pwdp;                     /* Использование теневого пароля */

    password = getpass("Password: ");
    
    /* Шифрование пароля с немедленным уничтожением незашифрованной версии */

    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0'; )
        *p++ = '\0';

    if (encrypted == NULL)
        errExit("crypt");

    authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOk) {
        printf("Incorrect password\n");
        exit(EXIT_FAILURE);
    }

    printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

    /* Здесь совершаем то, ради чего аутентифицировались... */

    exit(EXIT_SUCCESS);
}