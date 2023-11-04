#pragma once
#include "os/kernel/types.h"



#define RFLAGS_IF   0x00000200       // eflags寄存器中的if位为1
#define GET_RFLAGS(EFLAG_VAR) __asm__ __volatile__("pushfq; popq %0" : "=g" (EFLAG_VAR))




#define sti() 		__asm__ __volatile__ ("sti	\n\t":::"memory")
#define cli()	 	__asm__ __volatile__ ("cli	\n\t":::"memory")
#define nop() 		__asm__ __volatile__ ("nop	\n\t")
#define io_mfence() 	__asm__ __volatile__ ("mfence	\n\t":::"memory")