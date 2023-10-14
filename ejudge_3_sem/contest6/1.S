		.intel_syntax noprefix
		.text
		.global avx_sin

avx_sin:
		push rbp
        mov rbp, rsp

		mov rcx, -1

		mov rdi, 2 //index
		vmovupd ymm1, ymm0 //cuurent_mn
        vxorpd ymm2, ymm2, ymm2 //ans
        vxorpd ymm3, ymm3, ymm3
.LoopStart:

        vmovupd ymm3, ymm2 // prev_ans
        vaddpd ymm2, ymm2, ymm1 // current_ans


        cmp rdi, 30
        jz .LoopEnd


        vmulpd ymm1, ymm1, ymm0
        vmulpd ymm1, ymm1, ymm0

        mov rax, rdi
        imul  rcx

        vcvtsi2sd xmm4, xmm4, rax
        vdivpd ymm1, ymm1, ymm4

        add rdi,1

        vcvtsi2sd xmm4, xmm4, rdi
        vdivpd ymm1, ymm1, ymm4

        add rdi, 1

        jmp .LoopStart
.LoopEnd:
        vmovupd ymm0, ymm2
        pop rbp
		ret