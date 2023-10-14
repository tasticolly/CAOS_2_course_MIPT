.text
    .global sum

sum:
    mov x4, x1
    mov x1, 0
    b loopbegin

loopbegin:
    cmp x1, x4
    bge loopend
    mul x3, x1, 4
    add x3, x3, w2
    ldr x3, [x3]
    add w0, w0, w3
    add x1, x1, 1
    b loopbegin

loopend:
    ret