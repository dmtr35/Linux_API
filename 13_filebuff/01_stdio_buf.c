#include <stdio.h>              /* setvbuf, setbuf, BUFSIZ, setbuffer, fflush */
#include <fcntl.h>              /* open */
#include <unistd.h>             /* read, write */
#include "../lib/tlpi_hdr.h"

// Буферизация в библиотеке stdio

//  Функция setvbuf() позволяет выбрать способ буферизации, которую будет применять библиотека stdio.
//  Возвращает 0 при успешном завершении или ненулевое значение при ошибке.
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
/*  mode:
        _IONBF — не выполнять буферизацию ввода-вывода.
        _IOLBF — использовать построчную буферизацию ввода-вывода.
        _IOFBF — применять полностью буферизованный ввод-вывод.
*/

// ------------------------------------------------------------------------------

//  Функция setbuf() является надстройкой над setvbuf() и выполняет точно такую же задачу.
/*  buf выделяется на BUFSIZ байт, BUFSIZ определен в stdio.h и равен 8192  */
//  Эквивалент такого вызова: setvbuf(fp, buf, (buf != NULL) ? _IOFBF: _IONBF, BUFSIZ);
void setbuf(FILE *stream, char *buf);

// ------------------------------------------------------------------------------

//  Функция setbuffer() аналогична функции setbuf(), но позволяет вызывающему коду указать размер буфера buf.
//  Эквивалент такого вызова: setvbuf(fp, buf, (buf != NULL) ? _IOFBF : _IONBF, size);
void setbuffer(FILE *stream, char *buf, size_t size);

// ==============================================================================

//  Сброс буфера stdio
//  fflush() сбрасывает буфер вывода для указанного потока.
//  Возвращает при успешном завершении 0 или EOF при ошибке
/*  Если для stream указано значение NULL, то fflush() сбрасывает на диск
    содержимое всех буферов stdio, которые связаны с потоками вывода.  */
int fflush(FILE *stream);



#define BUF_SIZE 1024
static char buf[BUF_SIZE];
static char buf2[BUFSIZ];

int main(int argc, char *argv[])
{
    // int fd;
    // fd = open("startup", O_WRONLY);
    // write(fd, "abc", 3);
    // ==================================================

    if (setvbuf(stdout, NULL, _IONBF, 0) != 0)              // отключили буферизацию и сообщение выводится сразу
        errExit("setvbuf");
    printf("msg");
    printf("\n");

    if (setvbuf(stdout, buf, _IOFBF, BUF_SIZE) != 0)        // включаем буферизацию
        errExit("setvbuf");
    printf("msg");
    printf("\n");
    // ==================================================

    setbuf(stdout, NULL);                                   // отключили буферизацию
    printf("msg2");
    printf("\n");

    setbuf(stdout, buf2);                                   // включаем буферизацию
    printf("msg2");
    printf("\n");

    // ==================================================

    setbuffer(stdout, NULL, 0);                             // отключили буферизацию
    printf("msg3");
    printf("\n");

    setbuffer(stdout, buf, BUF_SIZE);                       // включаем буферизацию
    printf("msg3");
    printf("\n");

    // ==================================================

    fflush(stdout);                                         /* или fflush(NULL); для всех буферов stdio, которые связаны с потоками вывода */

    // ==================================================
    printf("msg4");
    printf("\n");


    return 0;
}