#include <setjmp.h>         /* setjmp, longjmp */
#include "/home/dm/WebstormProjects/c/Linux_API/lib/tlpi_hdr.h"
#include "/home/dm/WebstormProjects/c/Linux_API/lib/error_functions.h"


// Возвращает 0 при первом вызове, ненулевое значение при возвращении через longjmp()
// setjump устанавливает цель для последующего перехода
// при первом вызове возвращает 
int setjmp(jmp_buf env);
// longjmp() выполняет переход на цель
void longjmp(jmp_buf env, int val);


/* вызов setjmp() допускается только внутри достаточно простых вы-
ражений, не требующих промежуточных мест хранения данных.
- в качестве цельного управляющего выражения switch, while и т. д.
- качестве операнда унарного оператора ! (НЕ)
- в качестве части операции сравнения (==, !=, < и т. д.)
- в качестве обособленного вызова функции, не встроенного в какое-либо более сложное выражение

s = setjmp(env);     НЕВЕРНО!! 
*/

static jmp_buf env;

static void f2(void)
{
    longjmp(env, 2);
}

static void f1(int argc)
{
    if (argc == 1)
        longjmp(env, 1);
    f2();
}

int main(int argc, char *argv[])
{
    int res = setjmp(env);

    switch (res) {
    case 0:                                 /* Это возвращение после предварительного вызова setjmp() */
        printf("Calling f1() after initial setjmp()\n");
        f1(argc);                           /* Данная программа никогда не выполнит break из следующей строки,... */
        break;                              /* ... но хороший тон обязывает нас его написать.*/
    case 1:
        printf("We jumped back from f1()\n");
        break;
    case 2:
        printf("We jumped back from f2()\n");
        break;
    }

    exit(EXIT_SUCCESS);
}
