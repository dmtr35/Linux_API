#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Шифрование пароля и аутентификация пользователя, компиляция с ключем -lcrypt */
/* Аргумент salt, и шифруемый пароль состоят из символов, выбранных из 64-символьного набора [a-zA-Z0-9/.] */

/*  Возвращает указатель на статично выделенную строку, содержащую
    при успешном завершении зашифрованный пароль, или NULL при ошибке
    
    Функция возвращает указатель на статически выделенную 13-символьную строку, являющуюся зашифрованным паролем.*/
char* crypt(const char *key, const char *salt);

/*  Возвращает при успешном завершении указатель на статически
    размещаемую строку ввода пароля или NULL при ошибке */
char *getpass(const char *prompt);


int main()
{
    /* Data Encryption Standard (DES)
        11                      -   salt
        TTCiBUh2dQc             -   hash
    */

    char *tmp, *pass1, *pass2, *pass3;

    tmp = crypt("QQ6669977#", "11");
    pass1 = strdup(tmp);

    tmp = crypt("QQ6669977#", "22");
    pass2 = strdup(tmp);

    tmp = crypt("QQ6669977#", "33");
    pass3 = strdup(tmp);

    printf("1: %s\n", pass1);                           // 11TTCiBUh2dQc
    printf("2: %s\n", pass2);                           // 22cebdO5qdECs
    printf("3: %s\n", pass3);                           // 339q1UJEpgnXo

    free(pass1);
    free(pass2);
    free(pass3);

    // =========================================================================
    
    /*  yescrypt
        $y$                                         -   yescrypt
        j9T                                         -   параметры/rounds
        $L5dsbEh9whqDVMy3AOXun/$                    -   salt
        PZPOKNsMAN9POMlamehoCx9joWoKWr2gtD          -   hash
    */

    char *yescrypt_pass;
    
    yescrypt_pass = crypt("QQ6669977#", "$y$j9T$L5dsbEh9whqDVMy3AOXun/$");
    printf("md5pass1: %s\n", yescrypt_pass);                // $y$j9T$L5dsbEh9whqDVMy3AOXun/$jsEHujm1RbKrANtdP9y4nn7ifebReVKEMeXT8l0cmB6
    
    // =========================================================================
    
    /*  md5
    $1$ → MD5 crypt
    salt → до 8 символов
    hash → результат
    */
    char *md5_pass;
    
    md5_pass = crypt("QQ6669977#", "$1$salt$");
    printf("md5_pass: %s\n", md5_pass);                     // $1$salt$whSFNgNyDAjd0Njh6A1r4/
    
    // =========================================================================


    return 0;
}


