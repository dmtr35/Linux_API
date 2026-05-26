#include "tlpi_hdr.h"

/* Выводит 'msg' плюс значение sysconf() для 'name' */
static void sysconfPrint(const char *msg, int name)
{
    long lim;

    errno = 0;
    lim = sysconf(name);
    if (lim != -1) {                    /* Вызов прошел успешно, ограничение определено */
        printf("%s %ld\n", msg, lim);
    } else {
        if (errno == 0)
            /* Вызов прошел успешно, ограничение не определено */
            printf("%s (indeterminate)\n", msg);
        else                            /* Вызов не удался */
            errExit("sysconf %s", msg);
    }
}

int main(int argc, char *argv[])
{
    sysconfPrint("_SC_ARG_MAX:", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX:", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX:", _SC_NGROUPS_MAX);
    sysconfPrint("_SC_PAGESIZE:", _SC_PAGESIZE);
    sysconfPrint("_SC_RTSIG_MAX:", _SC_RTSIG_MAX);

    exit(EXIT_SUCCESS);
}
