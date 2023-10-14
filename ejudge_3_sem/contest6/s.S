.intel_syntax noprefix
    .text
    .global dot_product

dot_product:
    endbr64
    vpxor ymm0, ymm0, ymm0
    mov rax, 0
    sub rdi, 8
.loop_begin:
    mov rcx, rdi
    add rdi, rcx
    cmp rax, rdi
    sub rdi, rcx
    jg .almost_end

    vmovups ymm1, YMMWORD PTR [rsi + rax * 8]
    vmovups ymm2, YMMWORD PTR [rdx + rax * 8]
    vmulps ymm1, ymm1, ymm2
    vaddps ymm0, ymm0, ymm1

    add rax, 8
    jmp .loop_begin

.almost_end:
    add rdi, 8

.final_loop:
    mov rcx, 2
    imul rcx
    sub rdi, rax
    mov rcx, 4
    imul rcx
    mov rcx, 0
    add rsi, rax
    add rdx, rax
    cmp rcx, rdi
    jz .the_end
    vmovss xmm1, DWORD PTR [rsi + rcx * 4]
    vmovss xmm2, DWORD PTR [rdx + rcx * 4]
    mulps xmm1, xmm2
    addps xmm0, xmm1
    add rcx, 1
    jmp .final_loop

.the_end:
    vpxor ymm1, ymm1, ymm1
    vhaddps ymm0, ymm0, ymm1
    vhaddps ymm0, ymm0, ymm1
    vextractf128 xmm1, ymm0, 1
    addps xmm0, xmm1
    ret