.intel_syntax noprefix
    .global _start
    .text
_start:
    push rbp
    mov rbp, rsp

    mov rax, 12
    mov rdi, 0
    syscall
    mov r8, rax
    add rax, 10000000
    mov rdi, rax
    mov rax, 12
    syscall

    mov r9, r8
    mov BYTE PTR [r9], '\n'
    add r9, 1

.read_all:
    cmp rax, 100
    jl .pre
    mov rax, 0
    mov rdi, 0
    mov rsi, r9
    mov rdx, 100
    syscall
    add r9, rax
    jmp .read_all
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
    mov rax, 1
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
    mov rax, 60
    mov rdi, 0
    syscall