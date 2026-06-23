#include <locale.h>
#include <time.h>               /* time, ctime, gmtime, localtime, mktime, asctime */
#include <sys/time.h>           /* gettimeofday */
#include <stdio.h>              /* printf */
#include "../lib/tlpi_hdr.h"

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)

long time_ms(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");

    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int main(int argc, char *argv[])
{
    time_t t;
    struct tm *gmp, *locp;
    struct tm gm, loc;
    struct timeval tv;
    t = time(NULL);

    printf("Seconds since the Epoch (1 Jan 1970): %ld", (long) t);
    printf(" (about %6.3f years)\n", t / SECONDS_IN_TROPICAL_YEAR);

    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");

    printf(" gettimeofday() returned %ld secs, %ld microsecs\n", (long)tv.tv_sec, (long)tv.tv_usec);
    gmp = gmtime(&t);

    if (gmp == NULL)
        errExit("gmtime");

    gm = *gmp;  /* Сохранение локальной копии, так как содержимое, на которое указывает
                    *gmp, может быть изменено вызовом asctime() или gmtime() */

    printf("Broken down by gmtime():\n");
    printf(" year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm.tm_year, gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
    printf("wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);
    locp = localtime(&t);

    if (locp == NULL)
        errExit("localtime");

    loc = *locp;            /* Сохранение локальной копии */

    printf("Broken down by localtime():\n");
    printf(" year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", loc.tm_year, loc.tm_mon, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_sec);
    printf("wday=%d yday=%d isdst=%d\n\n", loc.tm_wday, loc.tm_yday, loc.tm_isdst);

    printf("asctime() formats the gmtime() value as: %s", asctime(&gm));            // Fri May 22 13:00:50 2026
    printf("ctime() formats the time() value as: %s", ctime(&t));                   // Fri May 22 16:00:50 2026
    printf("mktime() of gmtime() value: %ld secs\n", (long) mktime(&gm));           // 1779447650 secs
    printf("mktime() of localtime() value: %ld secs\n", (long) mktime(&loc));       // 1779454850 secs

    exit(EXIT_SUCCESS);
}