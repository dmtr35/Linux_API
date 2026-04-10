#include <setjmp.h>         /* setjmp, longjmp */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


int setjmp(jmp_buf env);
// longjmp() выполняет переход на цель
void longjmp(jmp_buf env, int val);


jmp_buf env;

void target_function() {
    printf("target_function: calling setjmp\n");

    if (setjmp(env) == 0) {
        printf("target_function: returning\n");
        return;
    }

    // Мы сюда НЕ должны безопасно попадать
    printf("target_function: after longjmp (UB!)\n");
    return;
}

int main() {
    target_function();

    printf("main: target_function already returned\n");

    printf("main: calling longjmp...\n");
    longjmp(env, 1);

    printf("main: should never reach here\n");
}