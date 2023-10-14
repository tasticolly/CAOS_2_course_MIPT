    .data
    .global R
R:
    .space 8

    .text
    .global calculate
calculate:

    ldr x0, A
    ldr x1, B
    ldr x2, C
    ldr x3, D

    mul x0, x0, x1
    mul x1, x2, x3
    add x0, x0, x1

    adr x1, R
    str x0, [x1]
    ret