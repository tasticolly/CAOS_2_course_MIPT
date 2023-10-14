#include <sys/syscall.h>
#define SIZE_OF_TEXT 671088641
#define SIZE_OF_BUFFER 1048576

//671088641 = 64 * 1024 * 1024
//1048576 = 1024 * 1024
    .intel_syntax noprefix
    .global _start
    .text

_start:
    push rbp
    mov rbp, rsp


    push r11
    push rcx

    push r12
    push r13
    push r14
    push r15

    mov rax, SYS_brk
    mov rdi, 0
    xor r12,r12 // counter

    syscall

    mov r13, rax  //adress heap
    add rax, SIZE_OF_TEXT

    mov rdi, rax
    mov rax, SYS_brk

    syscall

    mov byte ptr [r13], '\n'
    inc r12
.first_while:

    mov rax, SYS_read // read
    mov rdi, 0 //input
    mov rsi, r13
    add rsi, r12
    mov rdx, SIZE_OF_BUFFER

    syscall

    mov r14, rax
    add r12, r14
    cmp r14, SIZE_OF_BUFFER


    jz .first_while

    sub r12, 1
    cmp byte ptr [r13 + r12], '\n'
    jz .last_symb
    inc r12
    mov byte ptr [r13 + r12], '\n'

.last_symb:

    xor r14,r14
    xor r15,r15 // for if first iteration


.second_while:

    cmp r12, 0
    jz .end_second_while

    inc r14
    sub r12, 1

    cmp byte ptr [r13 + r12], '\n'

    jnz .second_while


    mov rax, SYS_write
    mov rdi, 1
    mov rsi, r13
    add rsi, r12
    inc rsi



.if_first_iteration:

    mov rdx, r14
    syscall
    mov r14, 0
    jmp .second_while

.end_second_while:

    mov rdi, r13
    mov rax, SYS_brk

    syscall

    mov rax, SYS_exit //exit
    mov rdi, 0

    syscall

    pop r15
    pop r14
    pop r13
    pop r12

    pop rcx
    pop r11

    pop rbp