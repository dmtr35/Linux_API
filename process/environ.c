#define _GNU_SOURCE     /* program_invocation_name, program_invocation_short_name */
#include <fcntl.h>      /* open */
#include <unistd.h>     /* getpid, getppid*/
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* getenv, putenv, setenv*/
#include <errno.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


// /proc/PID/cmdline - хранятся аргументы процесса
// /proc/PID/environ - хранятся environ процесса

// Возвращает указатель на строку (значение) или NULL, если такой переменной не существует
// получить переменную из среды
char *getenv(const char *name);         /* #include <stdlib.h> */

// Возвращает 0 при успешном завершении или ненулевое значение при ошибке
// изменить или добавить переменную к среде вызывающего ее процесса
int putenv(char *string);           /* #include <stdlib.h> */

// Возвращает 0 при успешном завершении или –1 при ошибке
// создает новую переменную среды !
/* setenv() не изменяет среду, если переменная, идентифицируемая
аргументом name, уже существует, а аргумент overwrite имеет значение 0.
Если у overwrite ненулевое значение, среда всегда изменяется */
int setenv(const char *name, const char *value, int overwrite);         /* #include <stdlib.h> */

// Возвращает 0 при успешном завершении или –1 при ошибке
// удаляет из среды переменную, идентифицируемую аргументом name
int unsetenv(const char *name);         /* #include <stdlib.h> */

// Возвращает 0 при успешном завершении или ненулевое значение при ошибке
// удалить целиком всю среду (не определена в SUSv3)
int clearenv(void);    /* #define _BSD_SOURCE или: #define _SVID_SOURCE */

#define MAX_READ 30
extern char **environ;              // реальная память процесса (не копия)

/* char *envp[] следует избегать, так как область видимости является локальной для функции main(), так же не указано в спецификации SUSv3 */
int main(int argc, char *argv[], char *envp[])  
{
    int fd, res, overwrite;
    pid_t pid;
    size_t size_pid, size_path;
    char *path, *user_name, *env_user;
    ssize_t num_read, num_write;
    char buf[MAX_READ + 1];
    char str_pid[10];

    pid = getpid();
    printf("pid: %d\n", pid);
    sprintf(str_pid, "%d", pid);
    // ==========================================
    
    env_user = getenv("USER");            // извлечи отдельные значения из среды процесса
    printf("%s\n", env_user);
    
    // ==========================================
    user_name = "USER=sever";             // изменить или добавить переменную
    res = putenv(user_name);
    // // res = putenv("USER");           // без '=' удаляется из списка переменных среды
    env_user = getenv("USER");            // извлечи отдельные значения из среды процесса
    printf("%s\n", env_user);
    // ==========================================
    
    overwrite = 1;                         // если overwrite = 0, то существующее значение, не будет перезаписано
    res = setenv("USER", "over", overwrite);
    env_user = getenv("USER");            // извлечи отдельные значения из среды процесса
    printf("%s\n", env_user);
    // ==========================================
    
    unsetenv("USER");                     // удаляет из среды переменную
    clearenv();                           // удалить целиком всю среду

    char **ep;
    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);
    // ==========================================


    size_pid = strlen(str_pid);
    size_path = size_pid + 15;

    path = malloc(size_path);
    snprintf(path, size_path, "/proc/%s/environ", str_pid);

    fd = open(path, O_RDONLY);
    if (fd == -1)
        errExit("open");

    // while(true)
    while((num_read = read(fd, buf, MAX_READ)) > 0) {
        for (size_t i = 0; i < num_read; i++) {
            if (buf[i] == '\0')
                buf[i] = '\n';
        }
        num_write = write(STDOUT_FILENO, buf, num_read);
        if (num_write == -1)
            errExit("write");
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}
