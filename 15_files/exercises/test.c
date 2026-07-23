#include <stdio.h>                      /* printf */
#include <sys/stat.h>                   /* umask, mode_t */

// mode_t umask(mode_t mask);

int main()
{
    int m = 0;

    m = umask(0);
    umask(m);
    printf("%o\n", m);


    return 0;
}