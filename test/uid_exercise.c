#define _GNU_SOURCE             /* getresuid, getresgid, setresuid, setresgid */
#include <unistd.h>             /* getuid, getgid, geteuid, getegid, seteuid, setegid,
                                   setreuid, setregid, getresuid, getresgid, setresuid,
                                   setresgid, getgroups */
#include <sys/fsuid.h>          /* setfsuid, setfsgid */
#include <sys/types.h>          /* uid_t, gid_t */
#include <stdio.h>              /* printf */
#include <shadow.h>
#include "../lib/tlpi_hdr.h"

void printf_uid(void)
{
    uid_t ruid, euid, suid, fsuid;

    if (getresuid(&ruid, &euid, &suid) == -1)
        errExit("getresuid");

    fsuid = setfsuid(0);

    printf("UID:    %d     %d      %d      %d\n", ruid, euid, suid, fsuid);
    printf("----------------------------------------------\n");
}

int main()
{
    
    int res;

    printf_uid();
    
    res = setresuid(0, 1000, 1000);
    printf_uid();
    
    
    struct spwd *spwd1;

    spwd1 = getspnam("root");
    if (spwd1 == NULL)
        errExit("getspnam");

    printf(
        "%-10s: %s\n"
        "%-10s: %s\n",
        "sp_namp", spwd1->sp_namp,                             /* Имя для входа в систему (имя пользователя) */
        "sp_pwdp", spwd1->sp_pwdp                             /* Зашифрованный пароль */
    );


    return 0;
}