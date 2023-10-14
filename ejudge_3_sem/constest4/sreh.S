.text
    .global main
main:
    sub sp, sp, 16
    str lr, [sp, 8]
    mov x20, xzr
    mov x19, 4
    mov x1, x19
    mov x0, xzr
    bl realloc
    str x0, [sp]

getting:
    cmp x19, x20
    bgt noneed
    ldr x0, [sp]
    mov x2, 2
    mul x19, x19, x2
    mov x1, x19
    bl realloc
    str x0, [sp]

noneed:
    bl getchar_unlocked
    cmp w0, -1
    beq putting
    ldr x1, [sp]
    strb w0, [x1, x20]
    add x20, x20, 1
    b getting

putting:
    cmp x20, xzr
    beq end
    sub x20, x20, 1
    ldr x1, [sp]
    ldrb w0, [x1, x20]
    bl putchar_unlocked
    b putting

end:
    ldr x0, [sp]
    bl free
    ldr lr, [sp, 8]
    add sp, sp, 16
    mov x0, xzr
    ret

.section .rodata