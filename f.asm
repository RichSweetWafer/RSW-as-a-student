 .section .data
    .str_flag:
    .string "%s"
    .int_flag:
    .string "%d"
    .global prefix
    .type prefix, @function
    .section .text
    
prefix: 
    pushq %rbp
    movq  %rsp, %rbp
    subq $48, %rsp

    movq %rdi, -40(%rbp)
    movl %esi, -44(%rbp)
    movl -44(%rbp), %eax
    movl $4, %esi
    movq %rax, %rdi
    call calloc@PLT
    movq %rax, -8(%rbp)
    movq $1, -24(%rbp)
    jmp .f_cond

.f_head:
    movq -24(%rbp), %rax
    salq $2, %rax
    leaq -4(%rax), %rdx 
    movq -8(%rbp), %rax
    addq %rdx, %rax
    movl (%rax), %eax
    movq %rax, -16(%rbp)
    jmp .w_cond

.w_body:
    movq -16(%rbp), %rax
    salq $2, %rax
    leaq -4(%rax), %rdx 
    movq -8(%rbp), %rax
    addq %rdx, %rax
    movl (%rax), %eax
    movq %rax, -16(%rbp)

.w_cond:
    movq -40(%rbp), %rdx
    movq -24(%rbp), %rax
    addq %rdx, %rax
    movzbl (%rax), %eax
    movq -16(%rbp), %rbx
    addq %rdx, %rbx
    movzbl (%rbx), %ebx
    cmpb %al, %bl
    je .if_cond 
    cmpq $0, -16(%rbp)
    jne .w_body
.if_cond: 
    movq	-40(%rbp), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %edx
	movq	-40(%rbp), %rcx
	movq	-16(%rbp), %rax
	addq	%rcx, %rax
	movzbl	(%rax), %eax
	cmpb	%al, %dl
	jne	.f_tail
    addq $1, -16(%rbp)
.f_tail: 
    movq -24(%rbp), %rbx
    salq $2, %rbx
    movq -8(%rbp), %rax
    addq %rbx, %rax
    movq -16(%rbp), %rdx
    movl %edx, (%rax)
    addq $1, -24(%rbp)
.f_cond:
    movl -44(%rbp), %eax
    
    cmpq %rax, -24(%rbp)
    jb .f_head
    movq -8(%rbp), %rax
    leave
    ret
    
    .global main
    .type main, @function
    
main:
    pushq %rbp
    movq  %rsp, %rbp
    subq $64, %rsp
    movl $5065003, %edi
    call malloc@PLT 
    movq %rax, -40(%rbp)
    movl $5050001, %edi
    call malloc@PLT
    movq %rax, -32(%rbp)
    movl $-1, -64(%rbp)
    movl $1, -60(%rbp) 
    movq -40(%rbp), %rax
    movq %rax, %rsi
    leaq .str_flag(%rip), %rdi
    movq $0, %rax
    call scanf@PLT
    movq -32(%rbp), %rax
    movq %rax, %rsi
    leaq .str_flag(%rip), %rdi
    movq $0, %rax
    call scanf@PLT
    movq -40(%rbp), %rax
    movq %rax, %rdi
    call strlen
    movq %rax, -24(%rbp)
    movq -32(%rbp), %rax
    movq %rax, %rdi
    call strlen
    movq %rax, -16(%rbp)
    movq -40(%rbp), %rdx
    movq -24(%rbp), %rax
    addq %rdx, %rax
    movb $32, (%rax)
    incq %rax
    movb $0, (%rax)
    movq -32(%rbp), %rax
    movq %rax, %rsi
    movq -40(%rbp), %rax
    movq %rax, %rdi
    call strcat@PLT
    movq -24(%rbp), %rax
    movl %eax, %edx
    movq -16(%rbp), %rax
    addl %eax, %edx
    addl $1, %edx
    movq -40(%rbp), %rax
    movl %edx, %esi
    movq %rax, %rdi
    call prefix
    movq %rax, -8(%rbp)
    movq $1, -48(%rbp)
    jmp .for_cond
.for_head:
    movq	-24(%rbp), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	leaq	0(,%rax,4), %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	cmpq	%rax, -24(%rbp)
    jne .increase 
    cmpl $0, -60(%rbp)
    je .comma 
    movl $0, -60(%rbp)
    jmp .res
.comma: 
    movl $44, %edi
    call putchar@PLT
.res:
    movq -48(%rbp), %rdx
    movl %edx, %eax
    movq -24(%rbp), %rdx
    subl %edx, %eax
    movl %eax, -64(%rbp)
    movl %eax, %esi
    leaq .int_flag(%rip), %rdi
    movl $0, %eax 
    call printf@PLT
.increase:
    addq $1, -48(%rbp)
.for_cond:
    movq -16(%rbp), %rax
    addq $1, %rax
    cmpq %rax, -48(%rbp)
    jb .for_head
    cmpl $-1, -64(%rbp)
    jne .mem_free
    movl -64(%rbp), %eax
    movl %eax, %esi
    leaq .int_flag(%rip), %rdi
    movl $0, %eax
    call printf@PLT
.mem_free:
    movq -8(%rbp), %rax
    movq %rax, %rdi
    call free@PLT
    movq -40(%rbp), %rax
    movq %rax, %rdi
    call free@PLT
    movq -32(%rbp), %rax
    movq %rax, %rdi
    call free@PLT
    movl $0, %eax
    leave
    ret

