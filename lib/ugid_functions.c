#include <pwd.h>
#include <stdio.h>
#include <grp.h>
#include <ctype.h>
#include <stdlib.h>


#include "ugid_functions.h"        /* Объявление определяемых здесь функций */
#include "tlpi_hdr.h"
#include "error_functions.h"


/* Возвращает имя, соответствующее 'uid', или NULL при ошибке */
char* userNameFromId(uid_t uid)
{
    struct passwd *pwd;

    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}


/* Возвращает идентификатор пользователя, соответствующего 'name', или -1 при ошибке */
uid_t userIdFromName(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;
    if (name == NULL || *name == '\0')                  /* Возвращает ошибку, если передан NULL*/
        return -1;                                      /* или пустая строка */

    u = strtol(name, &endptr, 10);                      /* если было передано строкой "1000" */
    if (*endptr == '\0')                                /* разрешение числовой строки */
        return u;
        
    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;
    
    return pwd->pw_uid;
}


/* Возвращает имя, соответствующее 'gid', или NULL при ошибке */
char* groupNameFromId(gid_t gid)
{
    struct group *grp;

    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}


/* Возвращает идентификатор группы, соответствующего 'name',или -1 при ошибке */
gid_t groupIdFromName(const char *name)
{
    struct group *grp;
    gid_t g;
    char *endptr;

    if (name == NULL || *name == '\0')          /* Возвращает ошибку, если передан NULL*/
        return -1;                              /* или пустая строка */

    g = strtol(name, &endptr, 10);              /* если было передано строкой "27" */
    if (*endptr == '\0')                        /* разрешение числовой строки */
        return g;

    grp = getgrnam(name);
    if (grp == NULL)
        return -1;

    return grp->gr_gid;
}



