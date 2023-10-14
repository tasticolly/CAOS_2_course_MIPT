   .intel_syntax noprefix

    .extern A
    .text                                  
    .globl very_important_function 


very_important_function:
    push r12
    push r13
    
    mov r12, rdi
    mov r13, 0

    sub rsp, 8

.LoopBegin:
    cmp r12, r13
    jz .LoopEnd

    lea    rdi, .format_scanf[rip]      //  load into `rdi` char* "%d"
    mov    rsi, rsp                  //  load into `rsi` pointer to int32_t on stack

    call   scanf                     //  call scanf

    movsxd rsi, DWORD PTR [rsp]      //  load into `rsi` int32_t value from stack
    mov    rcx, A[rip]               //  load into `rcx` address of A

    movsxd rcx, [rcx + 4 * r12]      //  load into `rcx` *(A + i * 4)
    imul   rsi, rcx                  //  store in `rax` value of (rsi * rcx)

    lea    rdi, .format_printf[rip]     //  load into `rdi` char* "%lld\n"

    call   printf                    // call printf

    inc    r12                       // increment `r12` i. e. `++i`
    jmp    .LoopBegin     


    
.LoopEnd:

    add rsp, 8
    pop r13
    pop r12

    ret

    .section .rodata
.format_scanf:
    .string "%d"
.format_printf:
    .string "%lld\n"



