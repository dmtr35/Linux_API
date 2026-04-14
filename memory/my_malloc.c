#define _GNU_SOURCE

#include <stdint.h>     /* intptr_t */
#include <unistd.h>     /* brk, sbrk*/
#include <stdlib.h>     /* malloc, calloc, realloc, free, posix_memalign */

#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


// int brk(void *end_data_segment);
// void *sbrk(intptr_t increment);

void *my_malloc(size_t size)
{
    size = (size + 15) & ~15;  // alignment

    void *cur = sbrk(0);

    if (brk((char*)cur + size) == -1)
        return NULL;

    return cur;
}

int main(int argc, char *argv[])
{
    char *base = sbrk(0);
    printf("Initial program break: %10p\n", base);
    // char *ptr = malloc(16);
    // printf("ptr: %p\n", ptr);
    // snprintf(ptr, 16, "%s", "hello_world!123");
    // printf("%s\n", ptr);

    // printf("Initial program break: %10p\n", sbrk(0));

    char *ptr_two = my_malloc(17);
    printf("ptr_two: %p\n", ptr_two);
    snprintf(ptr_two, 16, "%s", "hello_world!456");
    printf("%s\n", ptr_two);
    
    char *ptr_three = my_malloc(16);
    printf("ptr_three: %p\n", ptr_three);
    snprintf(ptr_three, 16, "%s", "hello_world!789");
    printf("%s\n", ptr_three);


    return 0;
}