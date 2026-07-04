#include <utime.h>                          /* utime */


//  Возвращает 0 при успешном завершении и -1 при ошибке
int utime(const char *pathname, const struct utimbuf *buf);

// struct utimbuf {
//     time_t actime;                      /* Время доступа */
//     time_t modtime;                     /* Время изменения */
// };

int main()
{

    return 0;
}