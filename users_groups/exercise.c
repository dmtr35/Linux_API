#define _GNU_SOURCE
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include "../lib/tlpi_hdr.h"




int main()
{
    struct passwd *pwd_getpwuid1, *pwd_getpwuid2;
    pwd_getpwuid1 = getpwuid(1000);
    pwd_getpwuid2 = getpwuid(999);

    // char *name1 = pwd_getpwuid->pw_name;
    // char *name2 = pwd_getpwuid->pw_name;
    // printf("%s %s\n", getpwuid(1000)->pw_name, getpwuid(999)->pw_name);

    return 0;
}