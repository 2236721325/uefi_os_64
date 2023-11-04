.extern handler_table

.section .text

.macro INTERRUPT_ENTRY idx,no_error_code
interrupt_entry_\idx:
    .if \no_error_code == 0
        pushq $0
    .endif
    pushq	%rax
    pushq	%rbx
    pushq	%rcx
    pushq	%rdx
    pushq	%rbp
    pushq	%rdi
    pushq	%rsi
    pushq	%r8
    pushq	%r9
    pushq	%r10
    pushq	%r11
    pushq	%r12
    pushq	%r13
    pushq	%r14
    pushq	%r15
    movq	%es,	%rax
    pushq	%rax

    movq	%ds,	%rax
    pushq	%rax

    movq	$0x10,	%rax
    movq	%rax,	%ds
    movq	%rax,	%es



    movq $\idx,%rax
    leaq handler_table(%rip), %rbx
    //fuction args
    movq %rsp,%rdi
    movq $\idx,%rsi

    call *(%rbx,%rax,8)
    jmp interrupt_exit


.endm

interrupt_exit:
    popq	%rax
    movq	%rax,	%ds
    popq	%rax
    movq	%rax,	%es
    popq	%r15
    popq	%r14
    popq	%r13
    popq	%r12
    popq	%r11
    popq	%r10
    popq	%r9
    popq	%r8
    popq	%rsi
    popq	%rdi
    popq	%rbp
    popq	%rdx
    popq	%rcx
    popq	%rbx
    popq	%rax
    addq	$0x08,	%rsp	
    iretq



// divide by zero
INTERRUPT_ENTRY 0x00,0
// debug
INTERRUPT_ENTRY 0x01, 0
// non maskable interrupt
INTERRUPT_ENTRY 0x02,0
// breakpoint
INTERRUPT_ENTRY 0x03,0
// overflow
INTERRUPT_ENTRY 0x04, 0
INTERRUPT_ENTRY 0x05, 0
INTERRUPT_ENTRY 0x06, 0
INTERRUPT_ENTRY 0x07, 0
//
INTERRUPT_ENTRY 0x08, 1
INTERRUPT_ENTRY 0x09, 0
INTERRUPT_ENTRY 0x0a, 1
INTERRUPT_ENTRY 0x0b, 1
//
INTERRUPT_ENTRY 0x0c, 1
INTERRUPT_ENTRY 0x0d, 1
INTERRUPT_ENTRY 0x0e, 1
INTERRUPT_ENTRY 0x0f, 0
//
INTERRUPT_ENTRY 0x10, 0
INTERRUPT_ENTRY 0x11, 1
INTERRUPT_ENTRY 0x12, 0
INTERRUPT_ENTRY 0x13, 0
//
INTERRUPT_ENTRY 0x14, 0
INTERRUPT_ENTRY 0x15, 1
INTERRUPT_ENTRY 0x16, 0
INTERRUPT_ENTRY 0x17, 0
//
INTERRUPT_ENTRY 0x18, 0
INTERRUPT_ENTRY 0x19, 0
INTERRUPT_ENTRY 0x1a, 0
INTERRUPT_ENTRY 0x1b, 0
//
INTERRUPT_ENTRY 0x1c, 0
INTERRUPT_ENTRY 0x1d, 0
INTERRUPT_ENTRY 0x1e, 0
INTERRUPT_ENTRY 0x1f, 0
//
INTERRUPT_ENTRY 0x20, 0
INTERRUPT_ENTRY 0x21, 0
INTERRUPT_ENTRY 0x22, 0
INTERRUPT_ENTRY 0x23, 0
INTERRUPT_ENTRY 0x24, 0
INTERRUPT_ENTRY 0x25, 0
INTERRUPT_ENTRY 0x26, 0
INTERRUPT_ENTRY 0x27, 0
INTERRUPT_ENTRY 0x28, 0
INTERRUPT_ENTRY 0x29, 0
INTERRUPT_ENTRY 0x2a, 0
INTERRUPT_ENTRY 0x2b, 0
INTERRUPT_ENTRY 0x2c, 0
INTERRUPT_ENTRY 0x2d, 0
INTERRUPT_ENTRY 0x2e, 0
INTERRUPT_ENTRY 0x2f, 0

// 下面的数组记录了每个中断入口函数的指针
.section .data
.global handler_entry_table
handler_entry_table:
    .quad interrupt_entry_0x00
    .quad interrupt_entry_0x01
    .quad interrupt_entry_0x02
    .quad interrupt_entry_0x03
    .quad interrupt_entry_0x04
    .quad interrupt_entry_0x05
    .quad interrupt_entry_0x06
    .quad interrupt_entry_0x07
    .quad interrupt_entry_0x08
    .quad interrupt_entry_0x09
    .quad interrupt_entry_0x0a
    .quad interrupt_entry_0x0b
    .quad interrupt_entry_0x0c
    .quad interrupt_entry_0x0d
    .quad interrupt_entry_0x0e
    .quad interrupt_entry_0x0f
    .quad interrupt_entry_0x10
    .quad interrupt_entry_0x11
    .quad interrupt_entry_0x12
    .quad interrupt_entry_0x13
    .quad interrupt_entry_0x14
    .quad interrupt_entry_0x15
    .quad interrupt_entry_0x16
    .quad interrupt_entry_0x17
    .quad interrupt_entry_0x18
    .quad interrupt_entry_0x19
    .quad interrupt_entry_0x1a
    .quad interrupt_entry_0x1b
    .quad interrupt_entry_0x1c
    .quad interrupt_entry_0x1d
    .quad interrupt_entry_0x1e
    .quad interrupt_entry_0x1f
    .quad interrupt_entry_0x20
    .quad interrupt_entry_0x21
    .quad interrupt_entry_0x22
    .quad interrupt_entry_0x23
    .quad interrupt_entry_0x24
    .quad interrupt_entry_0x25
    .quad interrupt_entry_0x26
    .quad interrupt_entry_0x27
    .quad interrupt_entry_0x28
    .quad interrupt_entry_0x29
    .quad interrupt_entry_0x2a
    .quad interrupt_entry_0x2b
    .quad interrupt_entry_0x2c
    .quad interrupt_entry_0x2d
    .quad interrupt_entry_0x2e
    .quad interrupt_entry_0x2f


