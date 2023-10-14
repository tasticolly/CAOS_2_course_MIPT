		.intel_syntax noprefix
		.text
		.global very_important_function

very_important_function:
		push rbp
        mov rbp, rsp
		push r15

		mov r15, 0
        vxorps ymm2, ymm2, ymm2
.LoopStart:
        cmp r15, rdi
        jz .LoopEnd
        vmovups ymm0, [rsi + r15 * 4]
        vmovups ymm1, [rdx + r15 * 4]

        vaddps ymm0, ymm0, ymm1

        vmovaps [rcx + r15 * 4], ymm0

        vxorps ymm3, ymm3, ymm3
        vhaddps ymm0, ymm0, ymm3
        vhaddps ymm0, ymm0, ymm3

        vaddps ymm2, ymm2, ymm0

        add r15, 8
        jmp .LoopStart
.LoopEnd:
        vextractf128 xmm1, ymm2, 1
        addps xmm2, xmm1

		vcvtss2sd xmm2, xmm2, xmm2
     	vcvtsi2sd xmm0, xmm0, rdi

		vdivsd xmm0, xmm2, xmm0
        pop r15
        pop rbp

		ret