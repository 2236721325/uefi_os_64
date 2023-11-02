#pragma once
#include "os/kernel/types.h"

typedef struct 
{
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
   uint32_t offset_3;        // offset bits 32..63
   uint32_t zero;            // reserved
}InterruptDescriptor64;

extern InterruptDescriptor64 IDT_Table[];
extern unsigned int TSS64_Table[26];

// void set_idt_gate(uint8_t n,uint64_t offset,uint16_t selector,uint8_t ist,uint8_t type_attributes);

// __attribute__((always_inline)) inline void set_intr_gate( uint32_t n,uint8_t ist,void * addr)
// {
//    set_idt_gate(n,(uint64_t)addr,0x08,ist,0x8E );	//P,DPL=0,TYPE=E
// }

// __attribute__((always_inline)) inline void set_trap_gate( uint32_t n,uint8_t ist,void * addr)
// {
//       set_idt_gate(n,(uint64_t)addr,0x08,ist,0x8F );//P,DPL=0,TYPE=F

// }


// __attribute__((always_inline)) inline void set_system_gate( uint32_t n,uint8_t ist,void * addr)
// {
//    set_idt_gate(n,(uint64_t)addr,0x08,ist,0xEF );//P,DPL=3,TYPE=F

// }

// __attribute__((always_inline)) inline void set_system_intr_gate(uint32_t n,uint8_t ist,void * addr)	//int3
// {
//    set_idt_gate(n,(uint64_t)addr,0x08,ist,0xEE );//P,DPL=3,TYPE=E

// }


#define _set_gate(gate_selector_addr,attr,ist,code_addr)	\
do								\
{	unsigned long __d0,__d1;				\
	__asm__ __volatile__	(	"movw	%%dx,	%%ax	\n\t"	\
					"andq	$0x7,	%%rcx	\n\t"	\
					"addq	%4,	%%rcx	\n\t"	\
					"shlq	$32,	%%rcx	\n\t"	\
					"addq	%%rcx,	%%rax	\n\t"	\
					"xorq	%%rcx,	%%rcx	\n\t"	\
					"movl	%%edx,	%%ecx	\n\t"	\
					"shrq	$16,	%%rcx	\n\t"	\
					"shlq	$48,	%%rcx	\n\t"	\
					"addq	%%rcx,	%%rax	\n\t"	\
					"movq	%%rax,	%0	\n\t"	\
					"shrq	$32,	%%rdx	\n\t"	\
					"movq	%%rdx,	%1	\n\t"	\
					:"=m"(*((unsigned long *)(gate_selector_addr)))	,					\
					 "=m"(*(1 + (unsigned long *)(gate_selector_addr))),"=&a"(__d0),"=&d"(__d1)		\
					:"i"(attr << 8),									\
					 "3"((unsigned long *)(code_addr)),"2"(0x8 << 16),"c"(ist)				\
					:"memory"		\
				);				\
}while(0)

#define load_TR(n) 							\
do{									\
	__asm__ __volatile__(	"ltr	%%ax"				\
				:					\
				:"a"(n << 3)				\
				:"memory");				\
}while(0)

__attribute__((always_inline)) inline void set_intr_gate(unsigned int n,unsigned char ist,void * addr)
{
	_set_gate(IDT_Table + n , 0x8E , ist , addr);	//P,DPL=0,TYPE=E
}

__attribute__((always_inline))  inline void set_trap_gate(unsigned int n,unsigned char ist,void * addr)
{
	_set_gate(IDT_Table + n , 0x8F , ist , addr);	//P,DPL=0,TYPE=F
}

__attribute__((always_inline)) inline void set_system_gate(unsigned int n,unsigned char ist,void * addr)
{
	_set_gate(IDT_Table + n , 0xEF , ist , addr);	//P,DPL=3,TYPE=F
}

__attribute__((always_inline)) inline void set_system_intr_gate(unsigned int n,unsigned char ist,void * addr)	//int3
{
	_set_gate(IDT_Table + n , 0xEE , ist , addr);	//P,DPL=3,TYPE=E
}
void set_tss64(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2,unsigned long ist3,
unsigned long ist4,unsigned long ist5,unsigned long ist6,unsigned long ist7);
void interrupt_init();



void interrupt_handle();


void exception_handle();


