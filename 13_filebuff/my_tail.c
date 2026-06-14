#include <stdio.h>                              /* printf */
#include <sys/stat.h>                           /* open: "S_IRUSR, S_IWUSR" */
#include <fcntl.h>                              /* open */
#include <string.h>                             /* strcmp, memset */
#include <stdlib.h>                             /* strtol */
#include <unistd.h>                             /* read, ssize_t, STDOUT_FILENO */
#include "../lib/error_functions.h"

#define MAX_READ 3

int main(int argc, char *argv[])
{
    int fd, buf_size = MAX_READ + 1, max_arr = 2;
    int n = 10, flag_f = 0, tot_line = 0;
    ssize_t res_read, res_write;
    char *file, *buf;
    off_t file_pos = 0;                     // абсолютная позиция в файле
    int *arr_points;

    if (argc < 2) {
        usageErr("%s [ -f ] [ -n num ] file\n", argv[0]);
    } else {
        file = argv[argc - 1];
        for(int i = 1; i < argc - 1; ++i) {
            char *arg = argv[i];

            if (strcmp(arg, "-f") == 0) {
                flag_f = 1;
                continue;
            }

            if (strncmp(arg, "-n", 2) == 0) {
                if (strcmp(arg, "-n") == 0) {
                    n = strtol(argv[++i], NULL, 10);
                    continue;
                }
                n = strtol(argv[i] + 2, NULL, 10);
            }
        }
    }
    
    // ===================================================

    fd = open(file, O_RDONLY);
    if (fd == -1)
        errExit("open");

    arr_points = calloc(max_arr, sizeof(int));
    if (arr_points == NULL)
        errExit("malloc");

    buf = malloc(buf_size * sizeof(char));
    if (buf == NULL) {
        free(arr_points);
        errExit("malloc");
    }

    while ((res_read = read(fd, buf, MAX_READ)) > 0) {
        buf[res_read] = '\0';

        for(int i = 0; i < res_read; ++i){
            if (buf[i] == '\n') {
                if(tot_line + 1 == max_arr) {
                    max_arr = max_arr * 2;
    
                    int *tmp = realloc(arr_points, max_arr * sizeof(int));
                    if(tmp == NULL) errExit("realloc");
    
                    arr_points = tmp;
                }
                arr_points[++tot_line] = file_pos + i + 1;
            }
        }
        file_pos += res_read;
    }
    
    if(res_read == -1) {
        free(buf);
        free(arr_points);
        errExit("read");
    }

    // ===================================================

    if (tot_line == 0) {
        // файл пустой, выходим
        free(buf); free(arr_points); close(fd);
        return 0;
    } else if (n > tot_line) {
        n = tot_line;
    }

    lseek(fd, arr_points[tot_line - n], SEEK_SET);

    while((res_read = read(fd, buf, MAX_READ)) > 0 || flag_f) {
        if(res_read == 0) {
            sleep(1);
            continue;
        }
        res_write = write(STDOUT_FILENO, buf, res_read);
    }

    free(buf);
    free(arr_points);
    close(fd);


    return 0;
}