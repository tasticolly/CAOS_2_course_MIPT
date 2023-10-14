.text
.global main
main:
    sub sp, sp, 16

    add x1, sp, 4

    str x30, [sp, 8]
    adr x0, format_str_signed
    bl scanf

    adr x0, format_str_signed
    mov x1, sp

    bl scanf

    ldr w2, [sp]
    ldr w3, [sp, 4]
    add w1, w2, w3
    adr x0, format_str_signed

    bl printf

    ldr x30, [sp, 8]
    add sp, sp, 16

    mov x0, 0
    ret

    .data
format_str_signed:
    .string "%d"