#include <sys/statfs.h>                     /* statfs */
#include <stdio.h>                          /* printf */
#include <stdlib.h>                         /* EXIT_SUCCESS */
#include "../lib/error_functions.h"

int main(int argc, char *argv[])
{
    struct statfs sfs;
    if (statfs("/dev/shm", &sfs) == -1)
        errExit("statfs");

    printf("File system type:              %#lx\n",      (unsigned long) sfs.f_type);
    printf("Optimal I/O block size:        %lu\n",       (unsigned long) sfs.f_bsize);
    printf("Total data blocks:             %lu\n",       (unsigned long) sfs.f_blocks);
    printf("Free data blocks:              %lu\n",       (unsigned long) sfs.f_bfree);
    printf("Free blocks for nonsuperuser:  %lu\n",       (unsigned long) sfs.f_bavail);
    printf("Total i-nodes:                 %lu\n",       (unsigned long) sfs.f_files);
    printf("File system ID:                %#x, %#x\n",  (unsigned) sfs.f_fsid.__val[0], (unsigned) sfs.f_fsid.__val[1]);
    printf("Free i-nodes:                  %lu\n",       (unsigned long) sfs.f_ffree);
    printf("Maximum file name length:      %lu\n",       (unsigned long) sfs.f_namelen);

    exit(EXIT_SUCCESS);
}