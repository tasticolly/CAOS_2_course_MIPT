	.file	"1.c"
	.intel_syntax noprefix
	.text
	.globl	SIZE_OF_CHUNK
	.section	.rodata
	.align 4
	.type	SIZE_OF_CHUNK, @object
	.size	SIZE_OF_CHUNK, 4
SIZE_OF_CHUNK:
	.long	8
	.text
	.globl	_start
	.type	_start, @function
_start:
.LFB0:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 40
	.cfi_offset 3, -24
	mov	rax, rsp
	mov	rbx, rax
	mov	eax, 8
	cdqe
	sub	rax, 1
	mov	QWORD PTR -32[rbp], rax
	mov	eax, 8
	cdqe
	mov	r8, rax
	mov	r9d, 0
	mov	eax, 8
	cdqe
	mov	rsi, rax
	mov	edi, 0
	mov	eax, 8
	cdqe
	mov	edx, 16
	sub	rdx, 1
	add	rax, rdx
	mov	edi, 16
	mov	edx, 0
	div	rdi
	imul	rax, rax, 16
	mov	rcx, rax
	and	rcx, -4096
	mov	rdx, rsp
	sub	rdx, rcx
.L2:
	cmp	rsp, rdx
	je	.L3
	sub	rsp, 4096
	or	QWORD PTR 4088[rsp], 0
	jmp	.L2
.L3:
	mov	rdx, rax
	and	edx, 4095
	sub	rsp, rdx
	mov	rdx, rax
	and	edx, 4095
	test	rdx, rdx
	je	.L4
	and	eax, 4095
	sub	rax, 8
	add	rax, rsp
	or	QWORD PTR [rax], 0
.L4:
	mov	rax, rsp
	add	rax, 0
	mov	QWORD PTR -40[rbp], rax
	mov	eax, 8
	cdqe
	mov	QWORD PTR -24[rbp], rax
	jmp	.L5
.L6:
	mov	edx, 8
	mov	rax, QWORD PTR -40[rbp]
	mov	ecx, edx
	mov	rdx, rax
	mov	esi, 0
	mov	edi, 0
	mov	eax, 0
	call	syscall@PLT
	mov	QWORD PTR -24[rbp], rax
	mov	rdx, QWORD PTR -24[rbp]
	mov	rax, QWORD PTR -40[rbp]
	mov	rcx, rdx
	mov	rdx, rax
	mov	esi, 1
	mov	edi, 1
	mov	eax, 0
	call	syscall@PLT
.L5:
	mov	eax, 8
	cdqe
	cmp	QWORD PTR -24[rbp], rax
	je	.L6
	mov	esi, 0
	mov	edi, 60
	mov	eax, 0
	call	syscall@PLT
	mov	rsp, rbx
	nop
	mov	rbx, QWORD PTR -8[rbp]
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_start, .-_start
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
