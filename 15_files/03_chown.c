#define _GNU_SOURCE
#include <unistd.h>             /* chown, lchown, fchown */





// =======================================================================
// Все вызовы возвращают 0 при успешном завершении и -1 при ошибке
/*  аргумент owner задает новый UID, а аргумент group — новый GID.
    Из-менить лишь один из этих идентификаторов можно так:
    указать значение -1 для другого аргумента, чтобы оставить его без изменений. */
int chown(const char *pathname, uid_t owner, gid_t group);
int lchown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);


int main(int argc, char *argv[])
{
    int res;

    res = chown("startup", -1, 4);
    return 0;
}