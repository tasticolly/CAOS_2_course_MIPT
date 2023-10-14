#include <sys/syscall.h>
#define SIZE_OF_CHUNK 8
    .intel_syntax noprefix
    .global _start
    .text

_start:
    push rbp
    push r13
    mov rbp, rsp
    mov r13, 8 // count
    sub rsp, 16

    push r11
    push rcx

.whileStart:

    mov rax, SYS_read // read
    mov rdi, 0 //input
    mov rsi, rsp
    mov rdx, SIZE_OF_CHUNK

    syscall

    mov r13, rax

    mov rax, SYS_write //write
    mov rdi, 1 //output
    mov rsi, rsp
    mov rdx, r13

    syscall

    cmp r13, SIZE_OF_CHUNK
    jz .whileStart


    mov rax, SYS_exit //exit
    mov rdi, 0

    syscall

    pop rcx
    pop r11

    pop r13
    pop rbp