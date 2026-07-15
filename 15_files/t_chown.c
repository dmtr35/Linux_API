#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>                             /* chown */
#include <string.h>                             /* strcmp */
#include <stdlib.h>                             /* EXIT_SUCCESS, EXIT_FAILURE */
#include <stdbool.h>                            /*  */

#include "../lib/ugid_functions.h"                     /* Объявление функций userIdFromName() и groupIdFromName() */
#include "../lib/error_functions.h"



int main(int argc, char *argv[])
{
    uid_t uid;
    gid_t gid;
    int j;
    bool errFnd;

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s owner group [file...]\n"
                "    owner or group can be '-', "
                "meaning leave unchanged\n", argv[0]);
    
    if (strcmp(argv[1], "-") == 0) {                    /* "-" ==> не менять владельца */
        uid = -1;
    } else {                                            /* Преобразовать имя пользователя в UID */
        uid = userIdFromName(argv[1]);
        if (uid == -1)
            fatal("No such user (%s)", argv[1]);
    }
    
    if (strcmp(argv[2], "-") == 0) {                    /* "-" ==> не менять группу */
        gid = -1;
    } else {                                            /* Преобразовать имя группы в GID */
        gid = groupIdFromName(argv[2]);
        if (gid == -1)
            fatal("No group user (%s)", argv[1]);
    }

    /* Изменить принадлежность всех файлов, указанных в остальных аргументах */
    errFnd = false;
    for (j = 3; j < argc; j++) {
        if (chown(argv[j], uid, gid) == -1) {
            errMsg("chown: %s", argv[j]);
            errFnd = true;
        }
    }

    exit(errFnd ? EXIT_FAILURE : EXIT_SUCCESS);
}