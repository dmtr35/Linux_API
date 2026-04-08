#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>                     /* setjmp, longjmp */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


/* Поскольку различные компиляторы используют различные приемы оптимизации,
в портируемых программах в тех функциях, которые вызывают setjmp(), ключевое
слово volatile должно указываться со всеми локальными переменными вышеупомя-
нутых типов.

Если компилятору GNU C задать ключ -Wextra (extra warnings — «дополнительные
предупреждения»)
*/

static jmp_buf env;

static void doJump(int nvar, int rvar, int vvar)
{
    printf("Inside doJump(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
    longjmp(env, 1);
}

int main(int argc, char *argv[])
{
    int nvar;
    register int rvar;                  /* По возможности выделяется в регистре */
    volatile int vvar;                  /* Смотрите текст */
    
    
    nvar = 111;
    rvar = 222;
    vvar = 333;

    if (setjmp(env) == 0) {             /* Код, выполняемый после setjmp() */
        nvar = 777;
        rvar = 888;
        vvar = 999;
        doJump(nvar, rvar, vvar);
    } else {                                /* Код, выполняемый после longjmp() */
        printf("After longjmp(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
    }

    exit(EXIT_SUCCESS);
}

