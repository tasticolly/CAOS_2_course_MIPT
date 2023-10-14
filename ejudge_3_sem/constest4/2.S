.text
.global main

main:
    sub sp, sp, 16
    str x30, [sp, 8]
.LoopBegin:
    bl getchar_unlocked
    cmp w0, -1

    beq .LoopEnd

    cmp w0, '0'
    blt .LoopBegin
    cmp w0, '9'
    bhi .LoopBegin
    bl putchar_unlocked
    b .LoopBegin
.LoopEnd:
    ldr x30, [sp, 8]
    add sp, sp, 16
    mov x0, 0
    ret