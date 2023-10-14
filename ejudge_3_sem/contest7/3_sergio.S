#include <sys/syscall.h>
    .intel_syntax noprefix
    .global _start
    .text
_start:
    push rbp
    mov rbp, rsp
    mov r10, 32

    mov rax, SYS_brk
    mov rdi, 0
    syscall

    mov r8, rax
    mov r9, r8
    mov BYTE PTR [r9], '\n'
    add r9, 1

.allocating:
    add r10, r10
    mov rdi, r9
    add rdi, r10
    mov rax, SYS_brk
    syscall

.read_all:
    mov rax, SYS_read
    mov rdi, 0
    mov rsi, r9
    mov rdx, r10
    syscall
    add r9, rax
    cmp rax, r10
    jl .pre
    jmp .allocating

.pre:
    mov BYTE PTR [r9], '\n'
    sub r9, 1
    cmp BYTE PTR [r9], '\n'
    jz .reading
    add r9, 1
.reading:
    mov rsi, r9
    cmp rsi, r8
    jle .exit
.loop:
    sub rsi, 1
    cmp BYTE PTR [rsi], '\n'
    jz .writing
    jmp .loop
.writing:
    mov rax, SYS_write
    mov rdi, 1
    mov rdx, r9
    sub rdx, rsi
    add rsi, 1
    syscall
    sub rsi, 1
    sub r9, rdx
    jmp .reading
.exit:
    pop rbp
    mov rax, SYS_exit
    mov rdi, 0
    syscall