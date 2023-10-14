    .intel_syntax noprefix
    .extern A
    .text
    .global very_important_function


very_important_function:
    push r12
    push r13
    push r14
    push r15
    
    mov r12, rdi
    mov r13, 0

    sub rsp, 8

.LoopBegin:
    cmp r12, r13
    jz .LoopEnd


    lea rdi, .format_scanf[rip]
    mov rsi, rsp
    
    call scanf
    
    movsxd rcx, [rsp]
    mov rbx, A[rip]
    movsxd  rbx, [rbx + 4 * r13]
    imul rcx, rbx

    mov rsi, rcx
    lea rdi, .format_printf[rip]

    call printf


    inc r13
    jmp .LoopBegin

    
.LoopEnd:
    xor rax, rax
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12

    ret

    .section .rodata
.format_scanf:
    .string "%d"
.format_printf:
    .string "%lld\n"