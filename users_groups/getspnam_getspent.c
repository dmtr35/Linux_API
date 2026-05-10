#include <shadow.h>         /* getspnam, getspent, setspent, endspent */

#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// Возвращает при успешном завершении указатель или NULL, если запись не найдена либо произошла ошибка
struct spwd *getspnam(const char *name);
// Возвращает указатель при успешном завершении или NULL в случае конца потока либо при ошибке
struct spwd *getspent(void);

void setspent(void);
void endspent(void);

int main()
{

    return 0;
}