#define _GNU_SOURCE             /* getresuid, getresgid */
#include <stdio.h>              /* printf, sprintf, snprintf */
#include <unistd.h>             /* getuid, geteuid, setuid, read */
#include <sys/types.h>          /* uid_t, gid_t */
// #include <sys/stat.h>
#include <shadow.h>             /* /etc/shadow */
#include <stdlib.h>             /* malloc */
#include <fcntl.h>              /* open */
#include <string.h>             /* strlen */

// #include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"

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
  // ----------------------------------------------
  
  uid_t uid, euid;
  int res;
  
  uid = getuid();
  euid = geteuid();
  printf("real_id: %d, efect_id: %d\n", uid, euid);
  
  printf_uid_status(pid);             

  // ===============================================================
  /* setuid меняет id безвозвратно */
  // res = setuid(1000);               // Uid:	1000	1000	1000	1000
  // res = setgid(1000);               // Gid:	1000	1000	0	    1000
  // printf_uid_status(pid);
  // res = setuid(0);                  // Uid:	1000	1000	1000	1000
  // res = setgid(0);                  // Gid:	1000	0	    0	    0
  // printf_uid_status(pid);
  
  // ---------------------------------------------------------------
  /*  способ для set-user-ID-root программы, чей действующий UID в этот момент равен 0,
  безвозвратно сбросить все полномочия */
  // if (setuid(getuid()) == -1)
  //   errExit("setuid");
  // printf_uid_status(pid);
  // ===============================================================
  
  
  
  
  // ===============================================================
  /* изменения своего действующего пользовательского идентификатора 
  (без изменения сохраненный установленного идентификатора */
  // res = seteuid(1000);                  // Uid:	1000	1000	0	1000
  // res = setegid(1000);                  // Gid:	1000	1000	0	1000
  // printf_uid_status(pid);
  // /* тоесть мы можем переключиться назад на привилегированный процесс */
  // res = seteuid(0);                     // Uid:	1000	0     0 0
  // res = setegid(0);                     // Gid:	1000	0     0 0
  // printf_uid_status(pid);
  // ===============================================================
  
  
  // ===============================================================
  /* Изменение реальных и действующих идентификаторов 
     передать -1 чтобы оставить без изменения */
  // res = setreuid(-1, 1000);               // Uid:	1000	1000	0	  1000
  // printf_uid_status(pid);
  // res = seteuid(0);                       // Uid:	1000	0	    0	  0
  // printf_uid_status(pid);

  // ---------------------------------------------------------------

  /* лишиться своего привилегированного состояния безвозвратно */
  // setreuid(getuid(), getuid());             // Uid:	1000	1000	1000	1000
  // printf_uid_status(pid);
  
  // ===============================================================
  
  /* Извлечение реального, действительного и сохраненного установленного идентификаторов */
  // int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
  uid_t ruid2, euid2, suid2;
  res = getresuid(&ruid2, &euid2, &suid2);
  printf("Uid:  %d  %d  %d\n", ruid2, euid2, suid2);       // Uid:  1000  0  0
  puts("-------------------------------------------");

  // ===============================================================

  /*  Изменение реального, действительного и сохраненного установленного идентификаторов
    Оба при успешном завершении возвращают 0 или -1 при ошибке */
  res = setresuid(0, 1000, 0);                              // Uid:	0	1000	0	1000
  printf_uid_status(pid);


  
  // pwd = getspnam("root");
  // printf("%s\n", pwd->sp_namp);
  // sleep(30003);

  return 0;
}
