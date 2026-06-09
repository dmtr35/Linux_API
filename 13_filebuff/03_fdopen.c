// #define _GNU_SOURCE
#include <stdio.h>                          /* fileno, fdopen */
#include <unistd.h>                         /* STDOUT_FILENO */
// #include <fcntl.h>
// #include <unistd.h>             /* dup */
// #include <string.h>             /* strlen */
// #include "../lib/tlpi_hdr.h"
// #include "../lib/error_functions.h"
// #include <sys/types.h>                          /* Определения типов, используемые многими программами */
// #include <stdio.h>                              /* Стандартные функции ввода-вывода */
// #include <stdlib.h>                             /* Прототипы наиболее востребованных библиотечных функций плюс константы EXIT_SUCCESS и EXIT_FAILURE */
// #include <errno.h>                              /* Объявление errno и определение констант ошибок */
// #include <string.h> 

//  Смешивание библиотечных функций и системных вызовов для файлового ввода-вывода


/*  Возвращает при успешном завершении дескриптор файла, или -1 при ошибке */
int fileno(FILE *stream);


/*  Возвращает при успешном завершении (новый) указатель файла или NULL при ошибке 
    Аргумент mode имеет то же предназначение, что и в функции fopen(),
    например r для чтения, w для записи или a для добавления.   */
FILE *fdopen(int fd, const char *mode);



int main(int argc, char *argv[])
{
    printf("To man the world is twofold, ");
    // fflush(NULL);
    write(STDOUT_FILENO, "in accordance with his twofold attitude.\n", 41);


    return 0;
}