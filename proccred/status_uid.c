#include <stdio.h>              /* printf, sprintf, snprintf */
#include <unistd.h>             /* getuid, geteuid, setuid, read */
#include <sys/types.h>          /* uid_t, gid_t */
// #include <sys/stat.h>
#include <shadow.h>             /* /etc/shadow */
#include <stdlib.h>             /* malloc */
#include <fcntl.h>              /* open */
#include <string.h>             /* strlen */


#define MAX_READ 64



void printf_uid_status(pid_t pid)
{
  char *str_pid, *file_name;
  char buffer[MAX_READ];
  int len_pid, len_file_name, fd;
  FILE *fp;
  ssize_t res;
  
  str_pid = malloc(16);
  sprintf(str_pid, "%d", pid);
  len_pid = strlen(str_pid);
  
  len_file_name = len_pid + 14;
  file_name = malloc(len_file_name);
  snprintf(file_name, len_file_name, "/proc/%s/status", str_pid);
  
  fp = fopen(file_name, "r");
  if (fp == NULL) {
    perror("fopen");
    return;
  }
  
  while (fgets(buffer, MAX_READ - 1, fp) != NULL)
    if ((strstr(buffer, "Uid") != NULL) || (strstr(buffer, "Gid") != NULL) || (strstr(buffer, "Group") != NULL))
      printf("%s", buffer);

  puts("===========================================");
}

int main()
{
  struct spwd *pwd;
  pid_t pid;
  
  pid = getpid();
  printf_uid_status(pid);
  // ----------------------------------------------
  
  uid_t real_id, effect_id;
  int res;
  
  real_id = getuid();
  effect_id = geteuid();
  printf("real_id: %d, efect_id: %d\n", real_id, effect_id);
  
  res = setuid(real_id);
  printf_uid_status(pid);



  // pwd = getspnam("root");
  // printf("%s\n", pwd->sp_namp);
  // sleep(30003);

  return 0;
}
