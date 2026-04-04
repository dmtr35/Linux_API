// #define _GNU_SOURCE
#include <fcntl.h>                  /* open */
#include <unistd.h>                 /* dup */
#include <sys/uio.h>                  /* S_IRUSR | S_IWUSR */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


// struct iovec {
//     void *iov_base;
//     size_t iov_len;
// };

int main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[4];
    char *file = argv[1];
    ssize_t full_size = 0, num_read, num_write;

    char *first_name = "Vadim\n";
    char *last_name = "Sever\n";
    char *phone = "(0354) 777-78-98\n";
    char *age = "66\n";

    char buf_first_name[20];
    char buf_last_name[20];
    char buf_phone[20];
    char buf_age[20];

    fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        errExit("open");

    iov[0].iov_base = first_name;
    iov[0].iov_len = strlen(first_name);
    iov[1].iov_base = last_name;
    iov[1].iov_len = strlen(last_name);
    iov[2].iov_base = phone;
    iov[2].iov_len = strlen(phone);
    iov[3].iov_base = age;
    iov[3].iov_len = strlen(age);
    full_size = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len + iov[3].iov_len;

    // num_write = writev(fd, iov, 4);
    // if (num_write == -1)
    //     errExit("writev");
    for (int i = 0; i < (sizeof(iov) / sizeof(iov[0])); i++) {
        num_write = write(fd, iov[i].iov_base, iov[i].iov_len);
        if (num_write == -1)
            errExit("num_write");
    }
    
    iov[0].iov_base = buf_first_name;
    iov[0].iov_len = strlen(first_name);
    iov[1].iov_base = buf_last_name;
    iov[1].iov_len = strlen(last_name);
    iov[2].iov_base = buf_phone;
    iov[2].iov_len = strlen(phone);
    iov[3].iov_base = buf_age;
    iov[3].iov_len = strlen(age);
    
    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("lseek");
    // num_read = readv(fd, iov, 4);
    // if (num_read == -1)
    //     errExit("readv");
    // if (num_read < full_size)
    //     printf("Read fewer bytes than requested\n");
    
    for (int i = 0; i < (sizeof(iov) / sizeof(iov[0])); i++) {
        int len = iov[i].iov_len;
        char *base = (char *)iov[i].iov_base;
        num_read = read(fd, base, len);
        if (num_read == -1)
            errExit("num_read");
        base[num_read] = '\0';
    }

    return 0;
}