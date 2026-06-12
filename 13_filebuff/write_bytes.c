/* write_bytes.c

    Запись байтов в файл. (Простая программа для сравнительного анализа файлового ввода-вывода.) 

    Usage: write_bytes file num-bytes buf-size 

    Записывает байты «num-bytes» в «file», используя размер буфера «buf-size». Для каждой записи(). 

    Если скомпилировано с параметром -DUSE_O_SYNC, открывает файл с флагом O_SYNC, 
    так что все изменения данных и метаданных сбрасываются на диск. 

    При компиляции с параметром -DUSE_FDATASYNC выполнять fdatasync() после каждой записи. 
    так что изменения данных (и, возможно, метаданных) сбрасываются на диск. 

    Если скомпилировано с -DUSE_FSYNC, выполняйте fsync() после каждой записи, чтобы 
    данные и метаданные сбрасываются на диск.
*/

/*
    dm@y510p:~/WebstormProjects/c/Linux_API$ cc 13_filebuff/write_bytes.c lib/get_num.c lib/error_function.c -o main && time ./main startup 10000000 1024

    real    0m0,020s
    user    0m0,004s
    sys     0m0,016s

    dm@y510p:~/WebstormProjects/c/Linux_API$ cc -DUSE_FDATASYNC 13_filebuff/write_bytes.c lib/get_num.c lib/error_function.c -o main && time ./main startup 10000000 1024

    real    0m12,737s
    user    0m0,050s
    sys     0m0,611s
    dm@y510p:~/WebstormProjects/c/Linux_API$ cc -DUSE_O_SYNC 13_filebuff/write_bytes.c lib/get_num.c lib/error_function.c -o ma
    in && time ./main startup 10000000 1024

    real    0m20,462s
    user    0m0,042s
    sys     0m0,715s
    dm@y510p:~/WebstormProjects/c/Linux_API$ cc -DUSE_FSYNC 13_filebuff/write_bytes.c lib/get_num.c lib/error_function.c -o mai
    n && time ./main startup 10000000 1024

    real    0m20,966s
    user    0m0,032s
    sys     0m0,773s
*/

#include <sys/stat.h>
#include <fcntl.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[])
{
    if (argc != 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file num-bytes buf-size\n", argv[0]);

    size_t numBytes = getLong(argv[2], GN_GT_0, "num-bytes");
    size_t bufSize = getLong(argv[3], GN_GT_0, "buf-size");

    char *buf = malloc(bufSize);
    if (buf == NULL)
        errExit("malloc");

    int openFlags = O_CREAT | O_WRONLY;

#if defined(USE_O_SYNC) && defined(O_SYNC)
    openFlags |= O_SYNC;
#endif

    int fd = open(argv[1], openFlags, S_IRUSR | S_IWUSR);                   /* -rw------- */
    if (fd == -1)
        errExit("open");

    size_t thisWrite, totWritten;
    for (totWritten = 0; totWritten < numBytes; totWritten += thisWrite) {
        thisWrite = min(bufSize, numBytes - totWritten);

        if (write(fd, buf, thisWrite) != (ssize_t) thisWrite)
            fatal("partial/failed write");

#ifdef USE_FSYNC
        if (fsync(fd))
            errExit("fsync");
#endif
#ifdef USE_FDATASYNC
        if (fdatasync(fd))
            errExit("fdatasync");
#endif
    }

    if (close(fd) == -1)
        errExit("close");
    exit(EXIT_SUCCESS);
}