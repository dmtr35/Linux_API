#define _GNU_SOURCE
#include <sys/utsname.h>                /* uname */
#include <stdio.h>                      /* printf */
#include "../lib/tlpi_hdr.h"



//  Возвращает 0 при успешном завершении или -1 при ошибке
int uname(struct utsname *utsbuf);

int main()
{
    struct utsname uts;
    if (uname(&uts) == -1)
        errExit("uname");

    printf(
        "%-12s: %s\n"
        "%-12s: %s\n"
        "%-12s: %s\n"
        "%-12s: %s\n"
        "%-12s: %s\n"
        "%-12s: %s\n",
        "sysname", uts.sysname,                         /* Название реализации                              "cat /proc/sys/kernel/ostype" */
        "nodename", uts.nodename,                       /* Имя узла в сети                                  "cat /proc/sys/kernel/hostname" */
        "release", uts.release,                         /* Идентификатор выпуска ОС                         "cat /proc/sys/kernel/osrelease" */
        "version", uts.version,                         /* Версия ОС                                        "cat /proc/sys/kernel/version" */
        "machine", uts.machine,                         /* Оборудование, на котором запущена система */
        "domainname", uts.domainname                    /* Доменное имя хоста NIS                           "cat /proc/sys/kernel/domainname" */
    );
    
    /*  вся информация из файлов /proc/sys/kernel/ostype, /proc/sys/kernel/osrelease, /proc/sys/kernel/version хранится в /proc/version
        cat /proc/version
        Linux version 5.15.0-179-generic (buildd@lcy02-amd64-040) (gcc (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #189-Ubuntu SMP Tue May 5 18:20:56 UTC 2026
    */






    return 0;
}
