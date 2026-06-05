#include <stdio.h>            /* printf */
#include <sys/time.h>         /* gettimeofday */
#include <stddef.h>           /* NULL */


int main()
{
    int res;
    struct timeval tv;
    
    res = gettimeofday(&tv, NULL);
    printf(
        "%-12s: %ld\n"
        "%-12s: %ld\n",
        "tv_sec", tv.tv_sec,               /* Количество секунд с 00:00:00, 1 янв 1970 UTC */
        "suseconds_t", tv.tv_usec          /* Дополнительные микросекунды (long int) */
    );

    return 0;
}