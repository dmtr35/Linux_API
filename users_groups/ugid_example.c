#include <pwd.h>
#include <stdio.h>
#include <grp.h>
#include <ctype.h>
#include <stdlib.h>


#include "/home/dm/WebstormProjects/c/Linux_API/lib/ugid_functions.h"        /* Объявление определяемых здесь функций */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"



int main()
{
    /* получить имя пользователя по uid */
    char *name = userNameFromId(1000);
    if (name == NULL)
        errExit("getpwuid");
        
        printf("%s\n", name);
        
    // --------------------------------------------------------------
    /* получить uid по имени пользователя или строчной uid */
    uid_t uid = userIdFromName(name);
    if (uid == -1)
        errExit("getpwnam");
    
    printf("%d\n", uid);

    uid = userIdFromName("1000");
    if (uid == -1)
        errExit("getpwnam");
    
    printf("%d\n", uid);
    // --------------------------------------------------------------
    /* получить имя группы по gid */
    char* group = groupNameFromId(27);
    if (group == NULL)
        errExit("getgrgid");
    
    printf("%s\n", group);

    // --------------------------------------------------------------
    /* получить git по имени пользователя */
    gid_t gid = groupIdFromName(group);
    if (gid == -1)
        errExit("getgrnam");
    
    printf("%d\n", gid);

    return 0;
}


