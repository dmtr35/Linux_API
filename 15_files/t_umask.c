#include <sys/stat.h>
#include <fcntl.h>                  /* open */
#include "../lib/file_perms.h"
#include "../lib/tlpi_hdr.h"

#define MYFILE  "startup"
#define MYDIR   "mydir"
#define FILE_PERMS      (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)                         // rw-rw----
#define DIR_PERMS       (S_IRWXU | S_IRWXG | S_IRWXO)                                   // rwxrwxrwx
#define UMASK_SETTING   (S_IWUSR | S_IXUSR |S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)      // -wx-wx-wx

int main(int argc, char *argv[])
{
    int fd;
    struct stat sb;
    mode_t u;

    umask(UMASK_SETTING);

    fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
    if (fd == -1)
        errExit("open-%s", MYFILE);

    if (mkdir(MYDIR, DIR_PERMS) == -1)
        errExit("mkdir-%s", MYDIR);

    u = umask(0);                           /* Извлекает (и очищает) значение параметра umask */

    if (stat(MYFILE, &sb) == -1)
        errExit("stat-%s", MYFILE);

    printf("Requested file perms:   %s\n", filePermStr(FILE_PERMS, 0));
    printf("Process umask:          %s\n", filePermStr(u, 0));
    printf("Actual file perms:      %s\n\n", filePermStr(sb.st_mode, 0));

    if (stat(MYDIR, &sb) == -1)
        errExit("stat-%s", MYDIR);

    printf("Requested dir. perms:   %s\n", filePermStr(DIR_PERMS, 0));
    printf("Process umask:          %s\n", filePermStr(u, 0));
    printf("Actual dir. perms:      %s\n", filePermStr(sb.st_mode, 0));

    if (unlink(MYFILE) == -1)
        errMsg("unlink-%s", MYFILE);

    if (rmdir(MYDIR) == -1)
        errMsg("rmdir-%s", MYDIR);

    exit(EXIT_SUCCESS);
}