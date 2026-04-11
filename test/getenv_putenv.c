#include <stdio.h>      /* printf */
#include <stdlib.h>     /* getenv, putenv, setenv*/
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

int putenv(char *string);
char *getenv(const char *name);

extern char **environ;

int my_setenv(const char *name, const char *value, int overwrite)
{
    char *result;

    if (overwrite == 0 && (result = getenv(name)) != 0)
        return -1;

    ssize_t leng_str = strlen(name) + strlen(value) + 2;
    char *string = malloc(leng_str);
    snprintf(string, leng_str, "%s=%s", name, value);


    if ((putenv(string)) != 0)
        return -1;

    result = getenv(name);

    if (result == NULL) {
        size_t len = strlen(name) + 15;
        char *res = malloc(len);
        snprintf(res, len, "err: %s don't exist!\n", name);
        return -1;
    }
    else {
        return 0;
    }
}

int my_unsetenv(const char *name)
{
    while (getenv(name) != NULL) {
        if (putenv((char *)name) != 0)
            return -1;
    }
    return 0;
}

int main(int argc, char *argv[])  
{
    char *env_user, *result;
    const char *name = "USER";
    const char *value = "sever";
    int overwrite = 1, res;

    res = my_setenv(name, value, overwrite);
    printf("%d\n", result);

    res = my_unsetenv(name);
    if (res != 0)
        printf("err: %s don't deleted\n", name);
    else
        printf("success\n");
}