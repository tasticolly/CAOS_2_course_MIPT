#include <sys/syscall.h>
#define SIZE_OF_CHUNK 60 * 1024 * 1024
    .intel_syntax noprefix
    .global _start
    .text

_start:
    push rbp
    mov rbp, rsp

    push r13
    push r14
    push r15
    push r11
    push rcx



    mov r14, 32 //capacity
    mov r15, 8 // curent size + 8

    mov r13, 8 //count

    mov rax, SYS_brk
    mov rdi, 0

    syscall

    mov r10, rax //adress heap

    mov rax, SYS_brk
    mov rdi, r14

    syscall



.whileStart:

    cmp r15, r14
    jg .realloc


    mov rax, SYS_read // read
    mov rdi, 0 //input
    mov rsi, r10
    mov rdx, SIZE_OF_CHUNK

    syscall

    mov r13, rax

    cmp r13, SIZE_OF_CHUNK
    jz .whileStart

    jmp .whileEnd

.realloc:

    shl r14, 1



    jmp .whileStart

.whileEnd:
    mov rax, SYS_exit //exit
    mov rdi, 0

    syscall

    pop rcx
    pop r11

    pop r13
    pop rbp