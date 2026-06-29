#include <stdio.h>                      /* printf */
#include <sys/stat.h>                   /* stat, lstat, fstat */
#include <time.h>                       /* ctime */

typedef struct save_time{
        time_t atime;
        time_t mtime;
        time_t ctime;
        time_t atime_parent;
        time_t mtime_parent;
        time_t ctime_parent;
    } save_time;

// tag_vector* create_vector(double x, double y)
// {
//     tag_vector *ptr = malloc(sizeof(tag_vector));
//     ptr->x = x;
//     ptr->y = y;
//     return ptr;
// }

void print_time(struct save_time *st)
{
    printf("Last file access:           %s", ctime(&st->st_atime));
    printf("Last file modification:     %s", ctime(&st->st_mtime));
    printf("Last status change:         %s", ctime(&st->st_ctime));            
}

save_time* check_Xtime(struct stat *sb)
{
    save_time *ptr = malloc(sizeof(save_time));
    ptr->atime = sb->st_atime;
    ptr->mtime = sb->st_mtime;
    ptr->ctime = sb->st_ctime;
}

void X_time()
{
    
}


int main(int argc, char *argv[])
{
    int res;
    struct stat sb;

    res = stat(argv[1], &sb);
    check_Xtime(&sb);

    print_time();


    return 0;
}