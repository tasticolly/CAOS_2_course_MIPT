	.arch armv8-a
	.file	"main.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
.L3:
	bl	getchar_unlocked
	strb	w0, [sp, 31]
	ldrb	w0, [sp, 31]
	cmp	w0, 47
	bls	.L3
	ldrb	w0, [sp, 31]
	cmp	w0, 57
	bhi	.L3
	ldrb	w0, [sp, 31]
	bl	putchar_unlocked
	b	.L3
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.2.0-17ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
