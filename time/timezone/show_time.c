#include <time.h>
#include <locale.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/curr_time.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"

#define BUF_SIZE 200

/* вызов программы с TZ
    ./show_time
    TZ=":Pacific/Auckland" ./show_time
*/
int main(int argc, char *argv[])
{
    time_t t;
    struct tm *loc;
    char buf[BUF_SIZE];

    if (setlocale(LC_ALL, "C") == NULL)
        errExit("setlocale");               /* Использование в преобразовании настроек локали */

    t = time(NULL);
        
    printf("ctime() of time() value is: %s", ctime(&t));                    // Sun May 24 16:46:05 2026
    loc = localtime(&t);
    if (loc == NULL)
        errExit("localtime");

    printf("asctime() of local time is: %s", asctime(loc));                 // Sun May 24 16:46:05 2026
    if (strftime(buf, BUF_SIZE, "%A, %d %B %Y, %H:%M:%S %Z", loc) == 0)
        fatal("strftime returned 0");
    printf("strftime() of local time is: %s\n", buf);                       // Sunday, 24 May 2026, 16:46:05 EEST

    exit(EXIT_SUCCESS);
}