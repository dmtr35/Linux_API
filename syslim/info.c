#define _GNU_SOURCE
#include <unistd.h>             /* sysconf, pathconf, fpathconf */
#include <stdio.h>              /* printf */
#include <limits.h>

#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// Извлечение в ходе выполнения программы значений ограничений (и возможностей) системы
/*  _SC_* → sysconf()
    _PC_* → pathconf()  */

/*  Функция sysconf() позволяет приложению получить значения системных ограничений в ходе выполнения программы

    Возвращает значение ограничения, указанного в аргументе name, при успешном завершении или -1,
    если ограничение не определено или же если произошла ошибка */
long sysconf(int name);

/*
Название ограничения    Минимальное значение    название (<unistd.h>)   Описание
ARG_MAX                 4096                    _SC_ARG_MAX             Максимальное количество байтов для аргументов (argv) и для переменных среды (environ). которое может быть предоставлено exec()
Не определено           Не определено           _SC_CLK_TCK             Единица измерения для times()
LOGIN_NAME_MAX          9                       _SC_LOGIN_NAME_MAX      Максимальный размер имени для входа в систему (включая завершающий нулевой байт)
OPEN_MAX                20                      _SC_OPEN_MAX            Максимальное количество файловых дескрипторов, которые могут быть одновременно открыты процессом.
NGROUPS_MAX             8                       _SC_NGROUPS_MAX         Максимальное количество дополнительных идентификаторов групп, в которые может входить процесс
Не определено           1                       _SC_PAGESIZE            Размер страницы виртуальной памяти (синонимом является _SC_PAGE_SIZE)
RTSIG_MAX               8                       _SC_RTSIG_MAX           Максимальное количество различных сигналов реального времени
SIGQUEUE_MAX            32                      _SC_SIGQUEUE_MAX        Максимальное количество сигналов реального времени, поставленных в очередь
STREAM_MAX              8                       _SC_STREAM_MAX          Максимальное количество потоков стан дартного ввода-вывода, которые могут быть открыты одновременно
NAME_MAX                14                      _PC_NAME_MAX            Максимальное коли чество байтов в имени файла, не включая завершающий нулевой байт
                                                                        Для каталога это имя приводит к выдаче значения для файлов в каталоге. Поведение для других типов файлов не определено
PATH_MAX                256                     _PC_PATH_MAX            Максимальное количество байтов в путевом имени, включая завершающий нулевой байт
                                                                        Для каталога это имя приводит к выдаче максимальной длины относительного путевого имени из этого каталога. Поведение для других типов файлов не определено
PIPE_BUF                512                     _PC_PIPE_BUF            Максимальное количество байтов, которые могут быть атомарно записаны в конвейер или в FIFO
                                                                        Для FIFO-устройства или конвейера это имя приводит к выдаче значения, относящегося к указанному файлу. Для каталога это значение относится к FIFO-устройству, созданному в данном каталоге. Поведение для других типов файлов не определено
*/

// =======================================================================
//  получить значения ограничений, связанных с файлами
/*  Обе функции возвращают при успешном завершении значение ограничения,
    указанного с помощью аргумента name, или -1, если ограничение
    не определено или произошла ошибка  */
long pathconf(const char *pathname , int name);
long fpathconf(int fd, int name);



int main()
{
    long arg_max = sysconf(_SC_ARG_MAX);
    long clk_tck = sysconf(_SC_CLK_TCK);
    long login_name_max = sysconf(_SC_LOGIN_NAME_MAX);
    long open_max = sysconf(_SC_OPEN_MAX);
    long ngroups_max = sysconf(_SC_NGROUPS_MAX);
    long pagesize = sysconf(_SC_PAGESIZE);
    long rtsig_max = sysconf(_SC_RTSIG_MAX);
    long sigqueue_max = sysconf(_SC_SIGQUEUE_MAX);
    long stream_max = sysconf(_SC_STREAM_MAX);
    long name_max = pathconf("/", _PC_NAME_MAX);
    long path_max = pathconf("/", _PC_PATH_MAX);
    long pipe_buf = pathconf("/", _PC_PIPE_BUF);

    errno = 0;
    if (arg_max == -1) {
        if (errno == 0)
            printf("unlimited\n");
        else
            perror("sysconf");
        }

    printf(
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n"
        "%-15s: %ld\n",
        "_SC_ARG_MAX:", arg_max,                                // 2097152
        "_SC_CLK_TCK:", clk_tck,                                // 100
        "_SC_LOGIN_NAME_MAX:", login_name_max,                  // 256
        "_SC_OPEN_MAX:", open_max,                              // 1048576
        "_SC_NGROUPS_MAX:", ngroups_max,                        // 65536
        "_SC_PAGESIZE:", pagesize,                              // 4096
        "_SC_RTSIG_MAX:", rtsig_max,                            // 32
        "_SC_SIGQUEUE_MAX:", sigqueue_max,                      // 63034
        "_SC_STREAM_MAX:", stream_max,                          // 16
        "_PC_NAME_MAX:", name_max,                              // 255
        "_PC_PATH_MAX:", path_max,                              // 4096
        "_PC_PIPE_BUF:", pipe_buf                               // 4096
    );


    return 0;
}


/*  получить значения системных ограничений из оболочки
        dm@y510p:$ getconf ARG_MAX
        2097152
        dm@y510p:$ getconf LOGIN_NAME_MAX
        256
        dm@y510p:$ getconf OPEN_MAX
        1048576
        dm@y510p:~$ getconf NGROUPS_MAX
        65536
        dm@y510p:$ getconf RTSIG_MAX
        32
        dm@y510p:~$ getconf SIGQUEUE_MAX
        63034
        dm@y510p:$ getconf STREAM_MAX
        16
        dm@y510p:f NAME_MAX /
        255
        dm@y510p:f PATH_MAX /
        4096
        dm@y510pf PIPE_BUF /
        4096
*/