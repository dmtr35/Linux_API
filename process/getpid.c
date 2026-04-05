#include <fcntl.h>      /* open */
#include <unistd.h>     /* getpid getppid*/
#include <stdio.h>      /* printf */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

// Всегда успешно возвращает идентификатор вызывающего процесса
pid_t getpid(void);
// Всегда успешно возвращает идентификатор родительского процесса для того процесса, который его вызвал
pid_t getppid(void);

// в /proc/PID/status хранится информация о процесе, можно посмотреть ppid

int main()
{
    pid_t pid = getpid();
    printf("%d\n", pid);            // pid процеса ->1470459
    
    pid_t ppid = getppid();
    printf("%d\n", ppid);           // pid родителя процесса -> 1470443
    
}