#include <stdio.h>                      /* printf */
#include <time.h>                       /* printf */
#include <stdlib.h>                     /* malloc, strtol, rand, EXIT_SUCCESS */
#include <unistd.h>                     /* sleep, rmdir */
#include <sys/time.h>                   /* gettimeofday */
#include <fcntl.h>                      /* open */
#include <sys/stat.h>                   /* open: "S_IRUSR, S_IWUSR" */
#include <dirent.h>                     /* opendir, readdir */
#include <string.h>                     /* strcpy, strcmp */
#include <errno.h>
#include "../lib/error_functions.h"

#include <sys/stat.h>
#include <sys/types.h>


#define MAX_SIZE 64

long time_ms(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");

    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void sort(char **arr, int size_arr)
{
    char temp[MAX_SIZE];

    for (int i = 0; i < size_arr; ++i) {
        for (int j = 1+i; j < size_arr; ++j) {
            // printf("%s < %s\n", arr[j], arr[i]);
            if (strcmp(arr[j], arr[i]) < 0) {                      // знак '<' сортировка от меншего к большему, '>' наоботор
                strcpy(temp, arr[j]);
                strcpy(arr[j], arr[i]);
                strcpy(arr[i], temp);
            } else {
                continue;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int res, sr;
    long start, end;
    int num;
    char *dir;
    struct tm *tm;
    char **arr;
    srand(time(NULL));


    if (argc != 4)
        usageErr("%s num, dir\n", argv[0]);

    num = strtol(argv[1], NULL, 0);
    dir = argv[2];
    sr = strtol(argv[3], NULL, 0);

    arr = calloc(num, sizeof(char *));
    if (arr == NULL)
        errExit("malloc");

    if (mkdir(dir, 0777) == -1) {
        if (errno != EEXIST)
            errExit("mkdir");

        struct stat sb;

        if (stat(dir, &sb) == -1)
            errExit("stat");

        if (!S_ISDIR(sb.st_mode))
            fatal("%s exists but is not a directory", dir);
    }

    for(int i = 0; i < num; ++i){
        int n = 100000 + rand() % 900000;
        arr[i] = calloc(MAX_SIZE, sizeof(char));
        snprintf(arr[i], MAX_SIZE, "%s/x%d", dir, n);
    }

    // создание файлов
    start = time_ms();
    for (int i = 0; i < num; i++) {
        int fd = open(arr[i], O_CREAT | O_WRONLY, 0644);
        close(fd);
    }
    end = time_ms();
    printf("created in %ld ms\n", end - start);

    if (sr)
        sort(arr, num);

    // удаление файлов
    start = time_ms();
    for(int i = 0; i < num; ++i){
        int res = unlink(arr[i]);
    }
    end = time_ms();
    printf("deleted in %ld ms\n", end - start);

    if(rmdir(dir) == -1)
        errExit("rmdir");
    
    

    exit(EXIT_SUCCESS);
}
