// #define _GNU_SOURCE
#include <stdio.h>                          /* fileno, fdopen */
#include <unistd.h>                         /* STDOUT_FILENO */

//  Смешивание библиотечных функций и системных вызовов для файлового ввода-вывода


/*  Возвращает при успешном завершении дескриптор файла, или -1 при ошибке */
int fileno(FILE *stream);


/*  Возвращает при успешном завершении (новый) указатель файла или NULL при ошибке 
    Аргумент mode имеет то же предназначение, что и в функции fopen(),
    например r для чтения, w для записи или a для добавления.   */
FILE *fdopen(int fd, const char *mode);



int main(int argc, char *argv[])
{
    printf("To man the world is twofold, \n");
    write(STDOUT_FILENO, "in accordance with his twofold attitude.\n", 41);
    // fflush(NULL);
    // printf("To man the world is twofold, \n");
    // write(STDOUT_FILENO, "in accordance with his twofold attitude.\n", 41);


    return 0;
}