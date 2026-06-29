#ifndef FILE_PERMS_H
#define FILE_PERMS_H
#include <sys/types.h>

#define FP_SPECIAL 1    /* Включить в возвращаемую строку информацию о битах set-user-ID, set-group-ID и закрепляющем */

char *filePermStr(mode_t perm, int flags);

#endif