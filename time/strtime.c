#define _GNU_SOURCE                 /* strptime */
#include <time.h>                   /* strptime */
#include <locale.h>                 /* setlocale */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#define SBUF_SIZE 1000

// ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y"
// ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" "%F %T"

int main(int argc, char *argv[])
{
    struct tm tm;
    char sbuf[SBUF_SIZE];
    char *ofmt;
    char *res;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s input-date-time in-format [out-format]\n", argv[0]);

    if (setlocale(LC_ALL, "C") == NULL)
        errExit("setlocale");                           /* Использование настроек локали при преобразовании */

    memset(&tm, 0, sizeof(struct tm));                  /* Инициализация 'tm' */
    if (strptime(argv[1], argv[2], &tm) == NULL)
        fatal("strptime");
        
    tm.tm_isdst = -1;                       /* Не устанавливается функцией strptime(); заставляет функцию
                                               mktime() определить действие режима летнего времени */

    printf("calendar time (seconds since Epoch): %ld\n", (long) mktime(&tm));

    ofmt = (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";
    if (strftime(sbuf, SBUF_SIZE, ofmt, &tm) == 0)
        fatal("strftime returned 0");
    printf("strftime() yields: %s\n", sbuf);

    exit(EXIT_SUCCESS);
}