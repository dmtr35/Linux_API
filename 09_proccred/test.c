#define _GNU_SOURCE             /* getresuid */
#include <stdio.h>              /* printf */
#include <unistd.h>             /* getresuid */
#include <sys/types.h>          /* uid_t, gid_t */

void printf_uids(void)
{
    uid_t uid, euid, suid;

    getresuid(&uid, &euid, &suid);
    printf("Uid:    %d  %d  %d\n", uid, euid, suid);
}

int main()
{
    printf_uids();
    
    setresuid(1000, 1000, 1000);
    printf_uids();
    
    
    
    seteuid(999);
    printf_uids();


    return 0;
}

// gcc test.c ../lib/error_function.c -o test && sudo chown root test && sudo chmod u+s test && ./test