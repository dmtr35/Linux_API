; file: mini_longjmp.asm
; сборка:
; nasm -f elf64 mini_longjmp.asm && ld mini_longjmp.o -o mini_longjmp

section .bss
env:    resq 2        ; [rsp, rip]

section .text
global _start

; -------------------------
; my_setjmp(env)
; rdi = &env
; return 0
; -------------------------
my_setjmp:
    ; сохранить rsp
    mov [rdi], rsp

    ; сохранить адрес возврата (лежит на стеке)
    mov rax, [rsp]
    mov [rdi + 8], rax

    xor eax, eax      ; return 0
    ret

; -------------------------
; my_longjmp(env, val)
; rdi = &env
; rsi = val
; -------------------------
my_longjmp:
    ; восстановить rsp
    mov rsp, [rdi]

    ; загрузить rip
    mov rdx, [rdi + 8]

    ; вернуть значение (как будто setjmp вернул val)
    mov eax, esi

    ; ПРЫЖОК, НЕ ret!
    jmp rdx

; -------------------------
; entry point
; -------------------------
_start:

    lea rdi, [rel env]
    call my_setjmp

    cmp eax, 0
    jne after_jump

    ; ---- первый проход ----
    ; вызываем longjmp
    lea rdi, [rel env]
    mov esi, 42
    call my_longjmp

after_jump:
    ; сюда попадём после longjmp

    ; exit(0)
    mov rax, 60
    xor rdi, rdi
    syscall