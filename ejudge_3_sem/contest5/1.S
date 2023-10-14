    .intel_syntax noprefix
    .text
    .global sum

sum:
    mov r8, 0

.LoopBegin:
    cmp rdi, r8
    jz .LoopEnd

    mov eax, [rsi + 4 * r8]
    mov  ebx, [rdx + 4 * r8]
    add eax, ebx
    mov [rcx + 4 * r8], eax
    inc r8
    jmp .LoopBegin

.LoopEnd:
    ret