.text
.global main

main:
    mov x19, xzr
    mov x20, 16
    sub sp, sp, 16
    str x30, [sp, 8]
    mov x0, xzr
    mov x1, 16

    bl realloc
    str x0, [sp]

.LoopOneBegin:
    bl getchar_unlocked
    cmp w0, -1
    beq .LoopOneEnd

    mov w21, w0
    cmp x19, x20
    bne .IfEnd

    ldr x0, [sp]
    lsl x20, x20, 1
    mov x1, x20

    bl realloc

    str x0, [sp]

.IfEnd:

    ldr x0, [sp]
    strb w21, [x0, x19]
    add x19, x19, 1
    b .LoopOneBegin

.LoopOneEnd:
    sub x19, x19, 1
    ldr x0, [sp]
    ldrb w0, [x0, x19]
    bl putchar_unlocked
    cmp x19, xzr
    beq .LoopSecondEnd
    b .LoopOneEnd

.LoopSecondEnd:
    ldr x0, [sp]
    bl free
    ldr x30, [sp, 8]
    add sp, sp, 16
    mov x0, xzr
    ret