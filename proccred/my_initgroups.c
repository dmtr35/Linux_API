#define _GNU_SOURCE
#include <grp.h>                /* setgroups, getgrent, setgrent, endgrent */
#include <unistd.h>             /* getgroups */
#include <stdio.h>              /* printf */
#include <limits.h>             /* NGROUPS_MAX */
#include <string.h>             /* strstr */
#include <pwd.h>                /* getpwnam */

#define SG_SIZE (NGROUPS_MAX + 1)

int initgroups(const char *user, gid_t group);
/* getgrent(), setgrent() и endgrent() выполняют аналогичные задачи для файла групп */
struct group *getgrent(void);
void setgrent(void);
void endgrent(void);

void printf_uid_groups()
{
  gid_t grouplist[SG_SIZE];
  int res = getgroups(SG_SIZE, grouplist);
 
  printf("%s ", "Groups:");
  for (int i = 0; i < res; ++i)
    printf("%d ", grouplist[i]);
  putchar('\n');
}

int my_initgroups(const char *user, gid_t group)
{
    struct group *gr;
    gid_t grouplist[SG_SIZE];
    struct group *gr_getgrgid;
    struct passwd *pwd_getpwnam;
    int count = 0, res;

    while ((gr = getgrent()) != NULL) {
      gr_getgrgid = getgrgid(gr->gr_gid);

      for (char **mem = gr_getgrgid->gr_mem; *mem != NULL; mem++)
        if (strcmp(user, *mem) == 0) 
          grouplist[count++] = gr->gr_gid;
    }

    grouplist[count++] = getpwnam(user)->pw_gid;

    void endgrent(void);

    res = setgroups(count, grouplist);
    // printf("%d\n", res);

    // printf("Groups: ");
    // for (int i = 0; i < count; ++i)
    //   printf("%d ", grouplist[i]);
    
    // putchar('\n');
}


int main()
{
    int res;

    res = my_initgroups("dm", 1000);
    printf_uid_groups();

    return 0;
}

// gcc my_initgroups.c /home/dm/WebstormProjects/c/Linux_API/lib/error_function.c -o my_initgroups && sudo chown root my_initgroups && sudo chmod u+s my_initgroups && ./my_initgroups