#include <time.h>
#include "curr_time.h"                  /* Объявление определяемых здесь функций */

#define BUF_SIZE 1000

/* Возвращает строку, содержащую текущее время, отформатированное в сооответствии
со спецификацией в 'format' (спецификаторы на странице руководства strftime(3)).
Если 'format' имеет значение NULL, в качестве спецификатора мы используем "%c"
(что дает дату и время, как для ctime(3), но без завершающего символа новой строки).
При ошибке возвращается NULL. */

char* currTime(const char *format)
{
    static char buf[BUF_SIZE];              /* Нереентерабельная */
    time_t t;
    size_t s;
    struct tm *tm;
    
    t = time(NULL);
    tm = localtime(&t);

    if (tm == NULL)
        return NULL;
    s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);
    return (s == 0) ? NULL : buf;
}
