#include <stdio.h>                      /* printf */
#include <time.h>                       /* printf */
#include <stdlib.h>                     /* malloc, strtol, rand, EXIT_SUCCESS */
#include <unistd.h>                     /* sleep */
#include <fcntl.h>                      /* open */
#include <sys/stat.h>                   /* open: "S_IRUSR, S_IWUSR" */
#include <errno.h>
#include "../lib/error_functions.h"

#include <sys/stat.h>
#include <sys/types.h>


#define MAX_SIZE 64

int main(int argc, char *argv[])
{
    int res;
    time_t res1, diff;
    int num;
    char *dir;
    struct tm *tm;
    char **arr;
    srand(time(NULL));


    if (argc != 3)
        usageErr("%s num, dir\n", argv[0]);

    num = strtol(argv[1], NULL, 0);
    dir = argv[2];

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
        // printf("%s\n", arr[i]);
    }

    
    res1 = time(NULL);
    for(int i = 0; i < num; ++i){
        res = open(dir, O_RDONLY | O_CREAT);


    }
    
    
    diff = time(NULL) - res1;
    printf("%ld sec\n", (long)diff);

    exit(EXIT_SUCCESS);
}
