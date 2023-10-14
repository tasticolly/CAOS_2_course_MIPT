.text
.global sum

sum:
    mov x9, 0
.LoopBegin:
    sub x1, x1, 1
    ldr x8, [x2, x9]
    add w0, w0, w8
    add w9, w9, 4
    cmp xzr, x1
    beq .LoopEnd
    b .LoopBegin
.LoopEnd:
    ret