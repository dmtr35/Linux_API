#include <stdio.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* read, write */


int main(int argc, char *argv[])
{
    int fd;

    fd = open("startup", O_WRONLY);

    write(fd, "abc", 3);
    


    return 0;
}