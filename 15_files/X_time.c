#define _GNU_SOURCE                     /* realpath */
#include <limits.h>                     /* realpath */
#include <stdlib.h>                     /* realpath */
#include <libgen.h>                     /* dirname, basename */
#include <stdio.h>                      /* printf */
#include <sys/stat.h>                   /* stat, lstat, fstat, utimensat */
#include <sys/types.h>                  /* ino_t, dev_t, mode_t, uid_t, gid_t, off_t */
#include <time.h>                       /* ctime, strftime */
#include <string.h>                     /* strstr, strcmp, strdup */
#include <dirent.h>                     /* opendir, readdir */

#include <fcntl.h>                      /* open, O_WRONLY, O_CREAT, O_TRUNC */
#include <unistd.h>                     /* read, write */
#include <sys/stat.h>                   /* open: "S_IRUSR, S_IWUSR" */
#include <inttypes.h>                   /* uintmax_t */


#include "../lib/error_functions.h"

// int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags);

#define MAX_PATH 256
#define LIMIT 4096

typedef struct file_info {
    ino_t inode;
    dev_t device;
    mode_t mode;
    uid_t uid;
    gid_t gid;
    off_t size;
    struct timespec atime;
    struct timespec mtime;
    struct timespec ctime;
    char *path;
} file_info;

typedef struct database {
    struct file_info *files;
    size_t count;
    size_t capacity;
} database;

typedef struct a_m_time {
    char *atime_s;
    char *atime_ms;
    char *mtime_s;
    char *mtime_ms;
} a_m_time;


void free_database(struct database *db)
{
    for(int i = 0; i < db->count; ++i) {
        free(db->files[i].path);
    }
    free(db->files);

    db->files = NULL;
    db->count = 0;
    db->capacity = 0;
}




void print_time(const char *name, const struct timespec *ts)
{
    char buf[64];
    struct tm tm;

    localtime_r(&ts->tv_sec, &tm);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

    printf("%-15s %s.%09ld\n", name, buf, ts->tv_nsec);
}

void print_info(struct database *db)
{
    for (size_t i = 0; i < db->count; ++i) {
        printf("%-15s %s\n", "Path", db->files[i].path);
        printf("%-15s %ju\n", "I-node", (uintmax_t)db->files[i].inode);

        print_time("Access:", &db->files[i].atime);
        print_time("Modify:", &db->files[i].mtime);
        print_time("Change:", &db->files[i].ctime);

        puts("====================================================");
    }
}

void add_to_arr(char *path, struct stat *sb, database *db)
{
    if(db->capacity == db->count) {
        db->capacity *= 2;
        
        struct file_info *tmp = realloc(db->files, db->capacity * sizeof(struct file_info));

        if (tmp == NULL)
            errExit("realloc");

        db->files = tmp;
    }

    db->files[db->count].path = strdup(path);
    if (db->files[db->count].path == NULL)
        errExit("strdup");

    db->files[db->count].atime = sb->st_atim;
    db->files[db->count].ctime = sb->st_ctim;
    db->files[db->count].mtime = sb->st_mtim;
    db->files[db->count].inode = sb->st_ino;
    db->files[db->count].device = sb->st_dev;
    db->files[db->count].uid = sb->st_uid;
    db->files[db->count].gid = sb->st_gid;
    db->files[db->count].mode = sb->st_mode;
    db->files[db->count].size = sb->st_size;

    (db->count)++;
}

void func_tree(char *path, database *db)
{
    struct stat sb;
    DIR *info_dir;
    struct dirent *dirent;

    if(lstat(path, &sb) == -1)
        errExit("lstat");
    add_to_arr(path, &sb, db);

    if ((info_dir = opendir(path)) == NULL)
        errExit("opendir");
    
    
    while(dirent = readdir(info_dir)) {
        if(strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {
            size_t size_path = strlen(path) + strlen(dirent->d_name) + 2;
            char *new_path = malloc(size_path);                                         // выделана память
            if (new_path == NULL)
                errExit("malloc");

            snprintf(new_path, size_path, "%s/%s", path, dirent->d_name);

            if(lstat(new_path, &sb) == -1)
                errExit("lstat");

            if (S_ISDIR(sb.st_mode)) {
                func_tree(new_path, db);
            } else {
                add_to_arr(new_path, &sb, db);
            }
            free(new_path);
        }
    }
    closedir(info_dir);
}


void write_to_file(char *path, struct database *db, int flag_info)
{
    ssize_t res_wr;
    int fd;
    
    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR /* 0600 */);         /* -rw------- */
    if(fd == -1)
        errExit("open");

    for (size_t i = 0; i < db->count; ++i) {
        ssize_t res;
        struct file_info *f = &db->files[i];
        
        int len = snprintf(
            NULL,
            0,
            "%s|%ju|%ju|%ld|%09ld|%ld|%09ld\n",
            f->path,
            (uintmax_t)f->inode,
            (uintmax_t)f->device,
            (long)f->atime.tv_sec,  (long)f->atime.tv_nsec,
            (long)f->mtime.tv_sec,  (long)f->mtime.tv_nsec);
        if (len < 0)
            errExit("snprintf");

        char *str_data = malloc(len + 1);
        if (str_data == NULL)
            errExit("malloc");
        snprintf(
            str_data,
            len + 1,
            "%s|%ju|%ju|%ld|%09ld|%ld|%09ld\n",
            f->path,
            (uintmax_t)f->inode,
            (uintmax_t)f->device,
            (long)f->atime.tv_sec,  (long)f->atime.tv_nsec,
            (long)f->mtime.tv_sec,  (long)f->mtime.tv_nsec);

        res = write(fd, str_data, len);
        if (res == -1)
            errExit("write");

        // printf("%s\n", str_data);
        free(str_data);
    }

    if(flag_info)
        print_info(db);

    if(close(fd))
        errExit("close");
}


void set_timespec(void *ts, char *str_data, int flag_type)
{
    char *ptr;
    if((ptr = strrchr(str_data, '|')) != NULL) {
        if(flag_type == 0)
            *(time_t *)ts = strtol(&ptr[1], NULL, 10);
        else if (flag_type == 1)
            *(long *)ts = strtol(&ptr[1], NULL, 10);
        else if (flag_type == 2)
            *(dev_t *)ts = strtol(&ptr[1], NULL, 10);
        else if (flag_type == 3)
            *(ino_t *)ts = strtol(&ptr[1], NULL, 10);
        ptr[0] = '\0';
    }
}

void restor_meta(char *path_file, struct database *db, int flag_info)
{
    struct stat sb; 
    struct timespec times[2];
    int fd, offset = 0, count = 0, n = 256;
    ino_t saved_inode;
    dev_t saved_dev;
    ssize_t res;
    char buffer[MAX_PATH + 1] = {0};
    
    fd = open(path_file, O_RDONLY);
    if(fd == -1)
    errExit("open");
    
    while(res = read(fd, buffer, n)) {
        if((strchr(buffer, '\n')) == NULL) {
            n *= 2;
            if(lseek(fd, 0, SEEK_SET) == -1)
            errExit("lseek");
            if(n > LIMIT)
                fatal("limit memory");
            } else {
                char *ptr;
                size_t end = strcspn(buffer, "\n");
            char *str_data = malloc(end + 1);
            if(str_data == NULL)
            errExit("malloc");
            
            strncpy(str_data, buffer, end);
            str_data[end] = '\0';
            // printf("%s\n", str_data);
            
            // ===========================================================
            set_timespec(&times[1].tv_nsec, str_data, 1);
            set_timespec(&times[1].tv_sec, str_data, 0);
            set_timespec(&times[0].tv_nsec, str_data, 1);
            set_timespec(&times[0].tv_sec, str_data, 0);
            set_timespec(&saved_dev, str_data, 2);
            set_timespec(&saved_inode, str_data, 3);
            
            if(lstat(str_data, &sb) == -1)
                errExit("lstat");
            
            
            if (sb.st_ino == saved_inode && sb.st_dev == saved_dev) {
                if (utimensat(AT_FDCWD, str_data, times, 0) == -1)
                errExit("utimensat");
            }
            
            if(lstat(str_data, &sb) == -1)
                errExit("lstat");
            
            add_to_arr(str_data, &sb, db);
            // ===========================================================
            free(str_data);
            
            offset += end + 1;
            if(lseek(fd, offset, SEEK_SET) == -1)
            errExit("lseek");
        }
    }
    
    if(flag_info)
        print_info(db);  
}
void set_time(char *path_file, struct database *db,  a_m_time amt, int flag_info)
{
    struct stat sb;
    struct timespec times[2];
    int fd;

    char *str = "2024-07-06 16:30:13";

    struct tm tm = {0};
    if (strptime(str, "%Y-%m-%d %H:%M:%S", &tm) == NULL) {
        printf("Ошибка разбора\n");
        return;
    }

    time_t t = mktime(&tm);

    printf("%ld\n", (long)t);
}

split_time()
{

}

int main(int argc, char *argv[])
{
    int res;
    struct stat sb;
    char path[MAX_PATH];
    char path_file[MAX_PATH];
    char atime[32] = {0};
    char mtime[32] = {0};
    int flag_save = 0, flag_restor = 0, flag_settime, flag_info = 1;

    database db = {};
    db.count = 0;
    db.capacity = 2;
    db.files = malloc(db.capacity * sizeof(struct file_info));
    if (db.files == NULL)
        errExit("malloc");

    
    // ----------------------------------------------------------
    char args[32] = "";
    size_t j = 0;
    int first_non_option = 1;

        for (; first_non_option < argc; ++first_non_option) {

        if (strcmp(argv[first_non_option], "--") == 0) {
            ++first_non_option;
            break;
        }

        if (argv[first_non_option][0] != '-')
            break;
        
        for (char *p = argv[first_non_option] + 1; *p; ++p)
            args[j++] = *p;
    }

    args[j] = '\0';
    // ----------------------------------------------------------


    for (char *p = args; *p; ++p) {
        if (*p == 'q')
            flag_info = 0;
        else if (*p == 't')
            flag_settime = 1;
        else if (*p == 's')
            flag_save = 1;
        else if (*p == 'r')
            flag_restor = 1;
    }

    strcpy(path, argv[first_non_option]);
    if (argc - first_non_option == 2) {
        strcpy(path_file, argv[first_non_option + 1]);
    } else if (argc - first_non_option == 3) {
        strcpy(atime, argv[first_non_option + 1]);
        strcpy(mtime, argv[first_non_option + 2]);
    }

    if (flag_settime) {

        char *dot = strchr(atime, '.');
        char *atime_ms = strchr(atime, '.') + 1;
        char *end_mtime = strchr(mtime, '.');
        char *mtime_ms = strchr(mtime, '.') + 1;
        *end_atime = '\0';
        *end_mtime = '\0';
        a_m_time amt = {atime, atime_ms, mtime, mtime_ms};
        set_time(path_file, &db, amt, flag_info);
        return 0;
    } else if(flag_restor) {
        restor_meta(path_file, &db, flag_info);
        return 0;
    }

    char *full_path = realpath(path, NULL);                                             // выделяется память
    if (full_path == NULL)
        errExit("realpath");

    char *full_path_copy = strdup(full_path);                                           // выделяется память
    if (full_path_copy == NULL)
        errExit("strdup");

    char *parent_path = dirname(full_path_copy);            // обрезает full_path_copy (память не выделяется)


    if(lstat(parent_path, &sb) == -1)
        errExit("lstat");

    add_to_arr(parent_path, &sb, &db);
    free(full_path_copy);
    
    if(lstat(full_path, &sb) == -1)
        errExit("lstat");
    
    if (S_ISDIR(sb.st_mode)) {
        func_tree(full_path, &db);
    } else {
        add_to_arr(full_path, &sb, &db);
    }
    
    free(full_path);
          
    write_to_file(path_file, &db, flag_info);

    free_database(&db);
                
    return 0;
}