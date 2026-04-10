#define _GNU_SOURCE     /* program_invocation_name, program_invocation_short_name */
#include <fcntl.h>      /* open */
#include <unistd.h>     /* getpid, getppid*/
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* getenv, putenv, setenv*/
#include <errno.h>
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

extern char **environ;

int main(int argc, char *argv[])  
{

}