#define _GNU_SOURCE         /* Получение определения O_DIRECT из <fcntl.h> */
#include <fcntl.h>
#include <malloc.h>
#include "../lib/tlpi_hdr.h"

/*
    Программа выделяет блок памяти, который выровнен по адресу, кратному
    ее первому аргументу, и для этого использует функцию memalign().

    $ ./direct_read test.txt 512                    Считывание 512 байт со смещения 0
    Read 512 bytes                                  Успешно
    $ ./direct_read test.txt 256
    ERROR [EINVAL Invalid argument] read            Длина не кратна 512
    $ ./direct_read test.txt 512 1
    ERROR [EINVAL Invalid argument] read            Смещение не кратно 512
    $ ./direct_read test.txt 4096 8192 512
    Read 4096 bytes                                 Успешно
    $ ./direct_read test.txt 4096 512 256
    ERROR [EINVAL Invalid argument] read            Выравнивание не кратно 512
*/
int main(int argc, char *argv[])
{
    int fd;
    ssize_t numRead;
    size_t length, alignment;
    off_t offset;
    void *buf;

    if (argc < 3 || strcmp(argv[1], "-help") == 0)
        usageErr("%s file length [offset [alignment]]\n", argv[0]);

    length = getLong(argv[2], GN_ANY_BASE, "length");
    offset = (argc > 3) ? getLong(argv[3], GN_ANY_BASE, "offset") : 0;
    alignment = (argc > 4) ? getLong(argv[4], GN_ANY_BASE, "alignment") : 4096;

    fd = open(argv[1], O_RDONLY | O_DIRECT);
    if (fd == -1)
        errExit("open");

    /* Функция memalign() выделяет блок памяти, выровненный по адресу,
    кратному ее первому аргументу. Следующее выражение обеспечивает
    выравнивание 'buf' по границе, кратной 'alignment',
    но не являющейся степенью двойки. Это делается для того, чтобы в случае,
    к примеру, запроса буфера с выравниванием, кратным 256 байтам,
    не происходило случайного получения буфера, выровненного также
    и по 512-байтовой границе. Приведение к типу '(char *)' необходимо
    для проведения с указателем арифметических операций (что невозможно
    сделать с типом 'void *', который возвращает memalign(). */

    buf = (char *) memalign(alignment * 2, length + alignment) + alignment;
    
    if (buf == NULL)
        errExit("memalign");

    if (lseek(fd, offset, SEEK_SET) == -1)
        errExit("lseek");
    numRead = read(fd, buf, length);

    if (numRead == -1)
        errExit("read");
    printf("Read %ld bytes\n", (long) numRead);

    exit(EXIT_SUCCESS);
}