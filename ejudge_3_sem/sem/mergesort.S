.global buble_sort
.text

buble_sort:
    .FirstLoopBegin:
        mov x2, 0
        .SecondLoopBegin:
            mov x3, x2
            add x3, x3, 1
            madd x7, x3, 4, x0
            madd x7, x3, 4, x0
            ldr
        .SecondLoopEnd:
    .FirstLoopEnd:


