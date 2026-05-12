#include <pwd.h>                    /* getpwent, setpwent, endpwent */
#include <stdio.h>
#include <string.h>


struct passwd *getpwent(void);
void setpwent(void);
void endpwent(void);

struct passwd *my_getpwnam(const char *name)
{
    struct passwd *pw;

    setpwent();                         // перезапускает файл сначала

    while ((pw = getpwent()) != NULL) {
        if (strcmp(pw->pw_name, name) == 0) {
            return pw;
        }
    }

    endpwent();                         // закрывает файл
    return NULL;
}

int main()
{
    struct passwd *passwd1;

    passwd1 = my_getpwnam("dm");
    printf("%s\n", passwd1->pw_name);
    
    passwd1 = my_getpwnam("root");
    printf("%s\n", passwd1->pw_name);

}